<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>MetaData</title>
</head>

<style type="text/css">
.underline{
    border-bottom: 1px solid #000;
}
</style>

<body bgcolor="#042946">
</body>
</html>
<?php
$searchValue=$_REQUEST["searchValue"];
$file1=str_replace(" ","+",$searchValue)."\\".str_replace(" ","+",$searchValue)."+metadata.txt";
$file2=str_replace(" ","+",$searchValue)."\\".str_replace(" ","+",$searchValue)."+linkedin.txt";

@$MetadataFile = fopen($file1, "r");
@$LinkedinFile = fopen($file2, "r");

if($MetadataFile && filesize($file1))
{
	echo '<table class="table table-responsive" align="center" width="100%" bgcolor="#042946">';
	while(!feof($MetadataFile))
	{
		$temp = fgets($MetadataFile);
		echo '<tr><td width="25%" bgcolor="#042946"></td><td class="underline" align="center" bgcolor="#fff">';
		echo $temp.'<br>';
		echo '</td><td width="25%" bgcolor="#042946"></td></tr>';
	}
	echo '</table>';
}

if($LinkedinFile && filesize($file2))
{
	echo '<table class="table table-responsive" align="center" width="100%">';
	while(!feof($LinkedinFile))
	{
		$temp = fgets($LinkedinFile);
		echo '<tr><td width="25%" bgcolor="#042946"></td><td class="underline" align="center" bgcolor="#fff">';
		echo $temp.'<br>';
		echo '</td><td width="25%" bgcolor="#042946"></td></tr>';
	}
	echo '</table>';
}
else
{
	echo '<table class="table table-responsive" align="center" width="100%" bgcolor="#042946">';
	echo '<tr><td width="25%" bgcolor="#042946"></td><td class="underline" align="center" bgcolor="#fff">';
	echo "No social data found!";
	echo '</td><td width="25%" bgcolor="#042946"></td></tr>';
	echo '</table>';
}
?>