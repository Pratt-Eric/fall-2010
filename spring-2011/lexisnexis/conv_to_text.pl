#!/usr/bin/perl 


open (FILE,$ARGV[0]);
open (HEADOUTPUT, ">headnotetokens".$ARGV[1]);
open (FOOTOUTPUT, ">footnotetokens".$ARGV[1]);
open (CASEREFS, ">casereftokens".$ARGV[1]);
open (OUTPUT, ">out".$ARGV[1]);
open (CURRENT, ">current".$ARGV[1]);
open (TEST, "testing");

# Get LNI for the document
# This value is constant throughout the document.
sub getLNI
{
    my $lnistring = "";
    if ($_ =~ '<lncr:persistentidentifier>(.*?)<\/lncr:persistentidentifier>')
    {
	$lnistring = $lnistring.$1;
	$lnistring =~ s/\-//g;
	$lnistring =~ s/^(.*)$/$1:/;
    }
    return $lnistring;
}


# This routine helps in finding the Link citations. That is "This v. That" kind of citations.
# This captures and records in the casereftokens file.
# The format it captures is as follows
# Lni-current document::L_(citation_number)::TokenID::LNI-target document:: Actual citation

sub getLCitations
{
(my $randtemp, my $lnistring) = @_;
$i = 0;

while ($randtemp =~ /((.{200})(<lnci:cite ID=\"([^\"]*?)\"[^>]*?normprotocol=\"lexsee\"[^>]*>(.*?)<\/lnci:cite>))/g)
{
    $timepass = $2;
    $string = $3;
    $id = $4;
    $temp = $5;

    if ( $timepass =~ /lni=\"([A-Z0-9-]+)\"/)
    { $actuallni = $1; $actuallni =~ s/-//g; }
    else { $actuallni = ""; }
    $i++;
    $temp =~ s/<.*?>//g;
    $temp =~ s/<\/.*?>//g;
    $citation = $lnistring.":L_".$i."::".$id."::".$actuallni."::\t".$temp."\n";
    select CASEREFS;
    $| = 1;
    print CASEREFS $citation;
    $randtemp =~ s/\Q$string/ L_$i $temp /;
}
return $randtemp."\n";
}



# This routine helps in finding statutory citations in the document.
# The format it captures is as follows
# Lni-current document::S_(citation_number)::TokenID:: Actual citation

sub getSCitations
{
    $j = 0;
(my $actstring, my $lnistring) = @_;
while ($actstring  =~ /(<lnci:cite ID=\"([^\"]*?)\"[^>]*?normprotocol=\"lexstat\"[^>]*>(.*?)<\/lnci:cite>)/g)
{
    $j++;
    $string = $1;
    $token = $2;
    $temp = $3;
    $temp =~ s/<.*?>//g;
    $temp =~ s/<\/.*?>//g;
    select CASEREFS;
    $| = 1;
    print CASEREFS $lnistring.": S_".$j."::$token\t$temp\n";
    $actstring =~ s/\Q$string/ S_$j $temp /;
}
return $actstring;
}


#Keeps the count of the document being processed.
$count = 0;

# Action starts here.
# This routine is the entry point for the script.
# All other sub routines are connected through this main function.


while (<FILE>) {
#eliminate noise and weird characters. (Tried to do the best job I could)

    s/§/S/g;
s/ //g;
s/\$ /\$/g;
s/\&amp\;/\&/g;


my $lnistring = getLNI($_);
$count++;

#Owing to the size of the document space this keeps bogging, I have to always 
#flush the buffers, whenever I am using a 

select CURRENT;
$| = 1;
$mytemp = "processing file ".$count."\n";
#Writes the current file being processed information to the file "current"
print CURRENT $mytemp;



# The following functions capture both the court:representation part 
# and court:opinion part. First only the courtcase:opinion part was considered.
# Later we realized there are lots of cases with courtcase:representation part
# which is very large in size and also contains valuable citations.
# So courtcase:representation info was incorporated at a later stage.


if ($_ =~ /(<courtcase:representation>)(.*?)(<\/courtcase:representation>)/)
{
    $localstring = $2;
    getFinishedProduct($localstring, $lnistring);
}

if ($_ =~ /(<courtcase:opinion[^>]*?>)(.*)(<\/courtcase:opinion>)/)
{
    $localstring = $2;
    getFinishedProduct($localstring, $lnistring);
}

}


#This another major sub routine which calls citation and lni subroutines.
#Does all the work.
#The idea is quite simple. I take a whole document into a string.
# Then I start replacing anything I find unimportant carrying the string
# till the end of the routine and by the time I reach the end, I have a well
# refined, paragraph separated routine which I can finally document.

#FinishedProduct Sub Routine

sub getFinishedProduct {
(my $randtemp, my $lnistring) = @_;

$i = 0;
$finalstring = "";
$actstring = "";
$localstring = "";
$lcitation = "";
#Look for all citations with lni's involved.

$lcitation = getLCitations($randtemp,$lnistring);
print $lcitation; 
$caheadnote = "";
#Look for CA_Headnotes if its California document.
$headnote = $lcitation;


#Look for HNs for all other documents.

$i = 0;
while ($headnote =~ /(<casesum:headnote headnotesource=\"lexis-caselaw-editorial\">)(.*?)(<\/casesum:headnote>)/g)
{
    $temp = $2;
    if ($temp =~ /(<text>)(.*?)(<\/text>)/)
    {
	$i++;
	$string = $2;
	$string =~ s/<.*?>//g;
	$string =~ s/<\/.*?>//g;
	$mystring = "$lnistring:HN_$i $string\n\n";
	select HEADOUTPUT;			
	$| = 1;
	print HEADOUTPUT  $mystring;
	
    }
}
$i = 0;
while ($headnote =~ /(<casesum:headnote headnotesource=\"lexis-caselaw-editorial\">)(.*?)(<\/casesum:headnote>)/g)
{
    $i++;
    $headnote =~ s/(<casesum:headnote headnotesource=\"lexis-caselaw-editorial\">)(.*?)(<\/casesum:headnote>)/HN_$i /;
}


$caheadnote = $headnote;

while ($caheadnote =~ /((<casesum:headnote headnotesource=\"ca-official-reporter\">)(.*?)(<ref:anchor id=\"hnpara_([0-9]+)\"\/>)(.*?)(<\/casesum:headnote>))/g)
{
    $number = $5;
    $temp = $6;
    if ($temp =~ /(<text>)(.*?)(<\/text>)/)
    {
	$i = $number;
	$string = $2;
	$string =~ s/<.*?>//g;
	$string =~ s/<\/.*?>//g;
	
	if ( $string =~ / \" / ) {
	    $string =~ s/ \" ([^"]+\S)\"/ \"$1\"/g ;
	    $string =~ s/ \"(\S[^"]+) \" / \"$1\" /g ;
	    $string =~ s/ \" ([^"]+) \" / \"$1\" /g ;
	}
	select HEADOUTPUT;
	$| = 1;
	print HEADOUTPUT "$lnistring:CAL_HN_$i $string\n\n";
    }
}


while ($caheadnote =~ /((<casesum:headnote-ref.*?\">).*?<label>(.*?)<\/label>.*?(<\/casesum:headnote-ref>))/g)
{	
    $string = $3;
    $caheadnote =~ s/((<casesum:headnote-ref.*?\">).*?(<\/casesum:headnote-ref>))/CAL_HN_$string /;
}

$footnote = $caheadnote;
undef $caheadnote;

$i = 0;


#Look for all the footnotes within the document.

while ($footnote =~ /(<footnote>)(.*?)(<\/footnote)/g)
{
    $temp = $2;
    if ($temp =~ /(<text>)(.*?)(<\/text>)/)
    {
	$i++;
	$string = $2;
	$string =~ s/<.*?>//g;
	$string =~ s/<\/.*?>//g;
	$string =~ s/  +/ /g;
	$string =~ s/\( +/\(/g;
	
	if ( $string =~ / \" / ) {
	    $string =~ s/ \" ([^"]+\S)\"/ \"$1\"/g ;
	    $string =~ s/ \"(\S[^"]+) \" / \"$1\" /g ;
	    $string =~ s/ \" ([^"]+) \" / \"$1\" /g ;
	}

	select FOOTOUTPUT;
	$| = 1;
	print FOOTOUTPUT $lnistring.":FN_".$i." ".$string."\n\n";
    }
}



$i=0;
while ($footnote =~ /(<footnote>)(.*?)(<\/footnote>)/g)
{
    $i++;
    $footnote =~ s/(<footnote>)(.*?)(<\/footnote>)/FN_$i /;	
}
$actstring = $footnote;
undef $footnote;


# Look for statutory citations
$actstring = getSCitations($actstring,$lnistring);

$k=1;


while  ($actstring =~ /<p>(.*)<\/p>/g)
{
    $temp = $1;
    while ($temp =~ /<text>(.*?)<\/text>/g)
    {
	$i++;
	$string = $1;
	$string =~ s/<.*?>//g;
	$string =~ s/<\/.*?>//g;
	select TEST; $| = 1; print TEST $string;
	if ($string =~ /[?!(\.*\"*\'*\!*\:)]$/)
	{  $finalstring = $finalstring."vinayak   ".$string."\n\n"; }
	else { $finalstring = $finalstring."PARAGRAPH_".$k."\n".$string."\n\nvinayak"; $k++;}
    }
}

undef $temp;
undef $string;
undef $actstring;
$finalstring = "\n\n\n".$lnistring."\n\n".$finalstring;

# Remove double spaces and other unimportant things.

$finalstring =~ s/  +/ /g;
$finalstring =~ s/\( +/\(/g;

if ( $finalstring =~ / \" / ) {
    $finalstring =~ s/ \" ([^"]+\S)\"/ \"$1\"/g ;
    $finalstring =~ s/ \"(\S[^"]+) \" / \"$1\" /g ;
    $finalstring =~ s/ \" ([^"]+) \" / \"$1\" /g ;
}

# This part tries to avoid lni string being printed twice.
# If a document has both courtcase:representation and courtcase:opinion parts
# I tried to pass them both through this entire sub routine twice.
# This part of logic avoids this LNI of document to be printed twice.

if ($finalstring =~ /^[\s\n\r]*[A-Z0-9]{23}:[\s\r\n]*$/)
{ 
    select OUTPUT;
    $| = 1;
    print OUTPUT "";
}
else {
    select OUTPUT;
    $| = 1;
    print OUTPUT $finalstring;
}
undef $finalstring;
}


close FILE;
close OUTPUT;
close FOOTOUTPUT;
close HEADOUTPUT;
close CASEREFS;

