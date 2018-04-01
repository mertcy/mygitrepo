###############################################################################################################
# Language     :  PowerShell 2.0 (dns_map: PowerShell 5.0)
# Filename     :  recon.ps1 
# Autor        :  Oguz Mert Caylar (https://github.com/oguzmertcaylar)
# Description  :  Information gathering with powershell recon tool
# Repository   :  https://github.com/oguzmertcaylar/mygitrepo/recon
###############################################################################################################

<#
    .SYNOPSIS
    Information gathering with powershell recon tool
    .DESCRIPTION
    This tool consists of whois, traceroute, dnslookup, dnsmap functions.
    .EXAMPLE
    .\recon.ps1 <command> <domain>
    -------------------------------
	.\recon.ps1 whois mit.edu
	.\recon.ps1 traceroute mit.edu
	.\recon.ps1 dnslookup mit.edu
	.\recon.ps1 dnsmap mit.edu
    .LINK
    https://github.com/oguzmertcaylar/mygitrepo/tree/master/recon
#>

[CmdletBinding()]
Param(
	[Parameter(Mandatory = $true)][string]$command,
	[Parameter(Mandatory = $true)][string]$domain
)

function main {
	$startTime = Get-Date
	$report =  "information gathering with powershell recon tool report `r`n-------------------------------------------------------`n"
	$report | Set-Content  "report.txt"
	$report =  "command: $command $domain `r`n-------------------------------------------------------`n"
	$report | Add-Content "report.txt"
    	switch($command) {
        	"whois" { #starting who_is function
            		who_is $domain
			break
        	} "traceroute" { #starting trace_route function
            		trace_route $domain
			break
        	} "dnslookup" { #starting dns_look_up function
            		dns_look_up $domain
			break
        	} "dnsmap" { #starting dns_map function
            		dns_map $domain
			break
        	} default {
			Write-Host "`ncommand fault!`n" -ForegroundColor Red
			exit
        	}
    	}
	"-------------------------------------------------------`r`n" | Add-Content "report.txt"
	$endTime = Get-Date
	$runTime = $endTime - $startTime
        "start time: $startTime`r`n" | Add-Content "report.txt"
	"end time: $endTime`r`n" | Add-Content "report.txt"
        $runTime = 'duration: {0:ss} sec' -f $runTime
        $runTime | Add-Content "report.txt"	
}

function who_is($domain) {
	$port = 43
 	$server = "whois.internic.net"
	"port: $port `r" | Add-Content "report.txt"
	"server: $server `r`n" | Add-Content "report.txt"

	$socket = new-object Net.Sockets.TcpClient
	$socket.Connect($server, $port)

	if($socket.Connected) {
    		Write-Host "`nconnected to target " $domain "`n" -ForegroundColor Green
    		$stream = $socket.GetStream()
    
    		$writer = new-object System.IO.StreamWriter $stream
    		$line = $domain
            	$writer.WriteLine($line) 
            	$writer.Flush()

            	$buffer = new-object System.Byte[] 1024
            	$encoding = new-object System.Text.AsciiEncoding
           
            	do { 
                	try { 
                    		$read = $stream.Read($buffer, 0, 1024)
               
                    		if($read -gt 0) {                        		
                        		Write-Host $encoding.GetString($buffer, 0, $read)
					$encoding.GetString($buffer, 0, $read) | Add-Content "report.txt"
				}
                	} catch { 
                    		
                    		$read = 0
                	} 
            	} while($read -gt 0)
     
    		$socket.Close()
 
	} else {
    		Write-Host "unable to connect!" -ForegroundColor Red
		"unable to connect!" | Add-Content "report.txt"
	}

}

# trace the route between source computer and target machine
function trace_route($target) {
        $beginHop = 1
        $maxHops = 30
        $timeOut = 1000
	$message = new-object System.Byte[] 40;
 
    	try {
        	$ping = New-Object System.Net.NetworkInformation.Ping
        	$pingResult = $ping.Send($target)
		$messageLength = $message.Length	

		$ip = [System.Net.Dns]::GetHostAddresses($domain)[0].IPAddressToString
		$ipHost = [System.Net.Dns]::GetHostByAddress($ip).HostName

        	if($pingResult.Status -eq 'Success') {
			$info = "traceroute to $target, $maxHops hops max, $messageLength byte packets`n`n"
            		Write-Host $info -ForegroundColor Green
			$info | Add-Content "report.txt"
        	} else {
			$info = "unable to trace target system $target!"
			Write-Host $info -ForegroundColor Red
			$info | Add-Content "report.txt"
			exit
		}
    	} catch {
		$info = "unable to resolve target system $target!"
		Write-Host $info -ForegroundColor Red
		$info | Add-Content "report.txt"
        	exit
    	}

	$ResultList = @()
 
    	for($hop = $beginHop; $hop -lt $maxHops; $hop++) {
        	$pingOptions = new-object System.Net.NetworkInformation.PingOptions $hop, $true
        	$pingReply = $ping.Send($target, $timeOut, $message, $pingOptions)
 
        	if($pingReply.Address -eq $null -and $hop -ne 1) {
			$ip = '*'   
			$ipHost = '*'      		
        	} else {
            		$ip = $pingReply.Address
			try {
				$ipHost = [System.Net.Dns]::GetHostByAddress($ip).HostName
			} catch {}
        	}
 
        	$roundTripTime = $pingReply.RoundTripTime
        	$hopStatus = $pingReply.Status
  		
        	$Object = New-Object PSObject
        	$Object | Add-Member -MemberType NoteProperty -Name "hop" -Value $hop
        	$Object | Add-Member -MemberType NoteProperty -Name "roundTripTime (ms)" -Value $roundTripTime
        	$Object | Add-Member -MemberType NoteProperty -Name "status" -Value $hopStatus
        	$Object | Add-Member -MemberType NoteProperty -Name "host IP" -Value $ip
        	$Object | Add-Member -MemberType NoteProperty -Name "host name" -Value $ipHost
        	$ResultList += $Object
 
        	# current ip match target ip
        	if($pingReply.Address -eq $pingResult.Address) {
			break
		}
    	}
	$ResultList
	$ResultList | Add-Content "report.txt"
	Write-Host "trace complete!`n" -ForegroundColor Green
}

# function for finding out the IP address of a domain.
function dns_look_up($domain) {
	try {
		$ip = [System.Net.Dns]::GetHostAddresses($domain)[0].IPAddressToString
		$ipHost = [System.Net.Dns]::GetHostByAddress($ip).HostName
		$info = "`nIPv4 address for $domain"
    		Write-Host $info -ForegroundColor Green
		$info | Add-Content "report.txt"
		$info = "Domain Name Server: $ip `n"
    		Write-Host $info -ForegroundColor Green
		$info | Add-Content "report.txt"
		$info = "Domain Name: $ipHost `n"
    		Write-Host $info -ForegroundColor Green
		$info | Add-Content "report.txt"
	} catch {}
}

# function for finding out by brute-force if the subdomains of the domain exists in our dictionary.
function dns_map($domain) {
	#$DNSquery_A = (Resolve-DnsName -Name $domain -Type SOA).IPAddress | Where-Object {$_ -ilike "*"}
	#$DNSquery_AAAA = (Resolve-DnsName -Name $domain -Type SOA).IPAddress | Where-Object {$_ -ilike "*"}
	
    	$DNSquery_SOA = (Resolve-DnsName -Name $domain -Type SOA).NameAdministrator | Where-Object {$_ -ilike "*"}
	$DNSquery_MX = (Resolve-DnsName -Name $domain -Type MX).NameExchange | Where-Object {$_ -ilike "*"}
	$DNSquery_NS = (Resolve-DnsName -Name $domain -Type NS).NameHost | Where-Object {$_ -ilike "*"}

	$result_SOA = $DNSquery_SOA | Select
    	$result_MX = $DNSquery_MX | Select
	$result_NS = $DNSquery_NS | Select

	foreach($line in Get-Content .\dictionary.txt) {
		$regex = "$line.$domain"
    		foreach($res in $result_SOA) {
			if($res -match $regex) {
        			$info = "found SOA record for subdomain $res in the dictionary!`n"
        			Write-Host $info -ForegroundColor Green
				$info | Add-Content "report.txt"
    			}	
		}

    		foreach($res in $result_MX) {
			if($res -match $regex) {
        			$info = "found MX record for subdomain $res in the dictionary!`n"
        			Write-Host $info -ForegroundColor Green
				$info | Add-Content "report.txt"
    			}	
		}

    		foreach($res in $result_NS) {
			if($res -match $regex) {
				$info = "found NS record for subdomain $res in the dictionary!`n"
        			Write-Host $info -ForegroundColor Green
				$info | Add-Content "report.txt"
    			}	
		}
	}
}

#starting main function
main
