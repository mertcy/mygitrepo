this code implemented with powershell is a recon tool which has the functionality of gathering information about a domain.
this tool consists of whois, traceroute, dnslookup, dnsmap functions.
---------------------------------------------------------------------------------------------------------------------------
whois: deals with who is responsible for a domain name or an IP address.
every year, millions of individuals, businesses, organizations and governments register domain names, so each one must
provide identifying and contact information which may include: name, address, email, phone number,
and administrative and technical contacts. this information is often referred to as "whois data."
---------------------------------------------------------------------------------------------------------------------------
traceroute: is a utility that records the route (the specific gateway computers at each hop)
through the Internet between your computer and a specified destination computer. 
first of all a ping is used prior to using traceroute to see whether a host is present on the network.
then, the utility initiates the sending of a packet for each hop with a time limit value.
it also calculates and displays the amount of time each hop took. 
---------------------------------------------------------------------------------------------------------------------------
dnslookup: the process of finding out the IP address of any URL on the Internet.
---------------------------------------------------------------------------------------------------------------------------
dnsmap: used by pentesters during the information gathering/enumeration phase of infrastructure security assessments. 
during the enumeration stage, the security consultant would typically discover the target company’s IP netblocks,
domain names, phone numbers, etc …

in this function subdomain brute-forcing is used.
this technique should be used in the enumeration stage, as it’s especially useful when other domain enumeration techniques
such as zone transfers don’t work.
this function looks at SOA, MX, NS records of DNS and compares the subdomains of these records with the dictionary
by brute-force if it is found in the dictionary or not.
---------------------------------------------------------------------------------------------------------------------------
tool usage: PS .\recon.ps1 <command> <domain>
a report is automatically generated after each usage.
###########################################################################################################################
PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 whois mit.edu

connected to target  mit.edu

   Domain Name: MIT.EDU
   Registry Domain ID: 106_DOMAIN_EDU-VRSN
   Registrar WHOIS Server: whois.educause.net
   Registrar URL: http://www.educause.edu/edudomain
   Updated Date: 2017-05-24T07:04:01Z
   Creation Date: 1985-05-23T04:00:00Z
   Registry Expiry Date: 2018-05-23T04:00:00Z
   Registrar: Educause
   Registrar IANA ID: 365
   Registrar Abuse Contact Email:
   Registrar Abuse Contact Phone:
   Domain Status: clientDeleteProhibited https://icann.org/epp#clientDeleteProhibited
   Domain Status: clientTransferProhibited https://icann.org/epp#clientTransferProhibited
   Domain Status: clientUpdateProhibited https://icann.org/epp#clientUpdateProhibited
   Name Server: ASIA1.AKAM.NET
   Name Server: ASIA2.AKAM.NET
   Name Server: EUR5.AKAM.NET
   Name Server: NS1-173.AKAM.NET
   Name Server: NS1-37.AKAM.NET
   Name Server: USE2.AKAM.NET
   Name Server: USE5.AKAM.NET
   Name Server: USW2.AKAM.NET
   DNSSEC: unsigned
   URL of the ICANN Whois Inaccuracy Complaint Form: https://www.ic
ann.org/wicf/
>>> Last update of whois database: 2018-04-01T19:13:23Z <<<

PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 whois yahoo.com

connected to target  yahoo.com

   Domain Name: YAHOO.COM
   Registry Domain ID: 3643624_DOMAIN_COM-VRSN
   Registrar WHOIS Server: whois.markmonitor.com
   Registrar URL: http://www.markmonitor.com
   Updated Date: 2018-02-02T01:07:18Z
   Creation Date: 1995-01-18T05:00:00Z
   Registry Expiry Date: 2023-01-19T05:00:00Z
   Registrar: MarkMonitor Inc.
   Registrar IANA ID: 292
   Registrar Abuse Contact Email: abusecomplaints@markmonitor.com
   Registrar Abuse Contact Phone: +1.2083895740
   Domain Status: clientDeleteProhibited https://icann.org/epp#clientDeleteProhibited
   Domain Status: clientTransferProhibited https://icann.org/epp#clientTransferProhibited
   Domain Status: clientUpdateProhibited https://icann.org/epp#clientUpdateProhibited
   Domain Status: serverDeleteProhibited https://icann.org/epp#serverDeleteProhibited
   Domain Status: serverTransferProhibited https://icann.org/epp#serverTransferProhibited
   Domain Status: serverUpdateProhibited https://icann.org/epp#serverUpdateProhibited
   Name Server: NS1.YA
HOO.COM
   Name Server: NS2.YAHOO.COM
   Name Server: NS3.YAHOO.COM
   Name Server: NS4.YAHOO.COM
   Name Server: NS5.YAHOO.COM
   DNSSEC: unsigned
   URL of the ICANN Whois Inaccuracy Complaint Form: https://www.icann.org/wicf/
>>> Last update of whois database: 2018-04-01T19:19:24Z <<<
###########################################################################################################################
PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 dnslookup google.com

IPv4 address for google.com
Domain Name Server: 172.217.169.142

Domain Name: sof02s32-in-f14.1e100.net

PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 dnslookup yahoo.com

IPv4 address for yahoo.com
Domain Name Server: 98.137.246.8

Domain Name: media-router-fp2.prod1.media.vip.gq1.yahoo.com
###########################################################################################################################
PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 dnsmap yahoo.com
found NS record for subdomain ns1.yahoo.com in the dictionary!

found NS record for subdomain ns2.yahoo.com in the dictionary!

found NS record for subdomain ns3.yahoo.com in the dictionary!

found NS record for subdomain ns4.yahoo.com in the dictionary!

PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 dnsmap mit.edu
found SOA record for subdomain network-request.mit.edu in the dictionary!

found MX record for subdomain dmz-mailsec-scanner-1.mit.edu in the dictionary!

found MX record for subdomain dmz-mailsec-scanner-2.mit.edu in the dictionary!

found MX record for subdomain dmz-mailsec-scanner-3.mit.edu in the dictionary!

found MX record for subdomain dmz-mailsec-scanner-4.mit.edu in the dictionary!

found MX record for subdomain dmz-mailsec-scanner-5.mit.edu in the dictionary!

found MX record for subdomain dmz-mailsec-scanner-6.mit.edu in the dictionary!

found MX record for subdomain dmz-mailsec-scanner-7.mit.edu in the dictionary!

found MX record for subdomain dmz-mailsec-scanner-8.mit.edu in the dictionary!

PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 dnsmap google.com
found NS record for subdomain ns1.google.com in the dictionary!

found NS record for subdomain ns2.google.com in the dictionary!

found NS record for subdomain ns3.google.com in the dictionary!

found NS record for subdomain ns4.google.com in the dictionary!

found SOA record for subdomain dns-admin.google.com in the dictionary!
###########################################################################################################################
PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 traceroute google.com
traceroute to google.com, 30 hops max, 40 byte packets

hop                : 1
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 192.168.1.1
host name          : MitraStar.Home

hop                : 2
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 93.155.2.42
host name          : MitraStar.Home

hop                : 3
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 81.212.105.65
host name          : 81.212.105.65.static.turktelekom.com.tr

hop                : 4
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 81.212.213.204
host name          : 81.212.213.204.static.turktelekom.com.tr

hop                : 5
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 212.156.117.212
host name          : 212.156.117.212.static.turktelekom.com.tr

hop                : 6
roundTripTime (ms) : 0
status             : TimedOut
host IP            : *
host name          : *

hop                : 7
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 74.125.51.92
host name          : *

hop                : 8
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 108.170.250.177
host name          : *

hop                : 9
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 209.85.142.55
host name          : *

hop                : 10
roundTripTime (ms) : 58
status             : Success
host IP            : 216.58.206.174
host name          : sof02s27-in-f14.1e100.net

trace complete!

PS C:\Users\oguzmertcaylar\Desktop\recon> .\recon.ps1 traceroute yahoo.com
traceroute to yahoo.com, 30 hops max, 40 byte packets

hop                : 1
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 192.168.1.1
host name          : MitraStar.Home

hop                : 2
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 93.155.2.42
host name          : MitraStar.Home

hop                : 3
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 81.212.105.65
host name          : 81.212.105.65.static.turktelekom.com.tr

hop                : 4
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 81.212.213.204
host name          : 81.212.213.204.static.turktelekom.com.tr

hop                : 5
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 81.212.31.240
host name          : 81.212.31.240.35-hatay-t2-1.35-izmir-xrs-t2-1.statik.turktelekom.com.tr

hop                : 6
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 81.212.31.35
host name          : ceyhan-t4-1-adana-t3-3.turktelekom.com.tr

hop                : 7
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 212.156.140.69
host name          : 212.156.140.69.305-vie-col-3.34-ebgp-acibadem1-k.statik.turktelekom.com.tr

hop                : 8
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 62.115.38.33
host name          : win-b4-link.telia.net

hop                : 9
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 80.91.253.169
host name          : win-bb2-link.telia.net

hop                : 10
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 213.155.133.54
host name          : ffm-bb4-link.telia.net

hop                : 11
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 62.115.122.138
host name          : prs-bb4-link.telia.net

hop                : 12
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 80.91.251.100
host name          : nyk-bb4-link.telia.net

hop                : 13
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 62.115.137.59
host name          : chi-b21-link.telia.net

hop                : 14
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 62.115.117.48
host name          : sea-b2-link.telia.net

hop                : 15
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 62.115.61.122
host name          : yahoo-ic-328472-sea-b2.c.telia.net

hop                : 16
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 216.115.97.107
host name          : UNKNOWN-216-115-97-X.yahoo.com

hop                : 17
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 66.196.67.111
host name          : et-19-1-0.msr2.gq1.yahoo.com

hop                : 18
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 67.195.37.97
host name          : et-1-0-0.clr2-a-gdc.gq1.yahoo.com

hop                : 19
roundTripTime (ms) : 0
status             : TtlExpired
host IP            : 98.137.120.25
host name          : et-18-6.bas1-2-flk.gq1.yahoo.com

hop                : 20
roundTripTime (ms) : 250
status             : Success
host IP            : 98.137.246.8
host name          : media-router-fp2.prod1.media.vip.gq1.yahoo.com

trace complete!
