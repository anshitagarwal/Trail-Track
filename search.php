<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<link rel="stylesheet" href="dist/css/bootstrap.css">
<link rel="stylesheet" href="dist/css/bootstrap-glyphicons.css">
<script type="text/javascript" src="dist/css/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="dist/css/bootstrap.min.js"></script>
<title>Results</title>
</head>
<body>
</body>
</html>

<style type="text/css">
body {
    background-image:    url(background2.jpg);
    background-size:     cover; 
    background-repeat:   no-repeat;
    background-position: center center;
}
.textstyle{
    color: #FFFFFF;
	font-size:20px;
}
.round-button {
	width:150px;
}
.round-button-circle {
	width: 100%;
	height:0;
	padding-bottom: 50%;
    border-radius: 50%;
	border:5px solid #cfdcec;
    overflow:hidden;
    
    background: #4679BD; 
    box-shadow: 0 0 3px gray;
}
.round-button-circle:hover {
	background:#30588e;
}
.round-button a {
    display:block;
	float:left;
	width:100%;
	padding-top:25%;
    padding-bottom:50%;
	line-height:1em;
	margin-top:-0.5em;
    
	text-align:center;
	color:#e2eaf3;
    font-family:Verdana;
    font-size:1.2em;
    font-weight:bold;
    text-decoration:none;
}


</style>


<?php
$searchValue=$_REQUEST["searchValue"];
$searchTagValue=$_REQUEST["searchTagValue"];
$searchValue=str_replace(" ","+",$searchValue);
$searchTagValue=str_replace(" ","+",$searchTagValue);

exec(".\\DataCrawler\\summarize.exe $searchValue $searchTagValue",$output,$return);
exec(".\\DataCrawler\\summarizenews.exe $searchValue $searchTagValue",$output,$return);

$clickedTag=$_REQUEST["clickedTag"];
$clickedSecondTag=$_REQUEST["clickedSecondTag"];

$searchFile=str_replace(" ","+",$searchValue)."\\".str_replace(" ","+",$searchValue)."+report.txt";
$newsFile=str_replace(" ","+",$searchValue)."\\".str_replace(" ","+",$searchValue)."+newsreport.txt";
$myTags = fopen(".\\DataCrawler\\tags.txt", "r");
$myCorrectedTags = fopen(".\\DataCrawler\\correcttags.txt", "r");

$tagData = array();
$correcttagData = array();
$fileData = array();
$tagCounter=0;
$tagLineCounter=0;

while(!feof($myTags))
{
	$temp = fgets($myTags);
	$temp = trim($temp);
	$tagData[$tagCounter++] = explode(" ",$temp);	
}
$tagCounter=0;
while(!feof($myCorrectedTags))
{
	$temp = fgets($myCorrectedTags);
	$correcttagData[$tagCounter++] = $temp;	
}
$tagCounter=0;
@$myfile = fopen($searchFile, "r");
if($myfile)
while(!feof($myfile))
{
	$temp = fgets($myfile);
	$arr = str_split($temp);
	if(strcmp($arr[0],"$"))
	{
		$fileData[$tagLineCounter][0]=$temp;
		$fileData[$tagLineCounter][2]=0;
		$tagCounter++;
		$tagLineCounter++;
	}
	else
	{
		$temp = fgets($myfile);
		$tagLineCounter=$tagLineCounter-$tagCounter;
		for($i=0; $i<$tagCounter; $i++)
		{
			$fileData[$tagLineCounter++][1]=$temp;			
		}
		$tagCounter=0;
	}
}

//Print all formatted file data
/*
for($i=0; $i<$tagLineCounter; $i++)
{
	echo $fileData[$i][0];
	echo "  ";
	echo $fileData[$i][1];
	echo "<br>";
}
*/
//Print all tag Data
/*
for($i=0; $i<sizeof($tagData); $i++)
{
	for($j=0; $j<sizeof($tagData[$i]); $j++)
	{
		echo $tagData[$i][$j];
		echo " ";
	}
	echo "<br>";
}
*/
$searchValue = $_REQUEST["searchValue"];
$searchValue = str_replace(" ","+",$searchValue);

if(!$clickedTag)
{
	echo '<table class="table table-responsive"><tr><td></td><td align="center">';
	echo '<div class="round-button"><div class="round-button-circle"><a href=metadata.php?searchValue='.$searchValue.'>Metadata</a></td>';
	echo '<td></td><td>';
	echo '<div class="round-button"><div class="round-button-circle"><a href=news.php?searchValue='.$searchValue.'>&nbsp;&nbsp;&nbsp;&nbsp;News&nbsp;&nbsp;&nbsp;&nbsp;</a></td><td></td>';
	
	//Not clicked any tag or the first page
	for($i=0; $i<sizeof($tagData); $i++)
	{
		if($i%5==0)
			echo '</tr><tr>';
		echo '<td align="center">';
		echo ' <div class="round-button"><div class="round-button-circle"><a href=?searchValue='.$searchValue.'&clickedTag='.$tagData[$i][0].'>';
		//echo $tagData[$i][0];
		echo $correcttagData[$i];
		echo '</a></div></div>';
		echo '</td>';
	}
	echo '</tr></table>';
}
else
{
	$ifDataFound=0;		
	for($i=0; $i<sizeof($tagData); $i++)
	{
		if(strcmp($clickedTag,$tagData[$i][0])==0)
			break;
	}
	
	echo '<div align="center" class="textstyle"><h2>'. $correcttagData[$i].'</h2></div>';
		
	for($j=0; $j<sizeof($tagData[$i]); $j++)
	{
		for($k=0; $k<$tagLineCounter; $k++)
		{
			if(strpos($fileData[$k][0],$tagData[$i][$j])!==false)
			{
				$ifDataFound++;
				if($fileData[$k][2]==0)
				{
					echo '<div align="center" class="textstyle">'.$fileData[$k][1].'</div>';
					
					for($l=0; $l<$tagLineCounter; $l++)
					{
						if(strcmp($fileData[$k][1],$fileData[$l][1])==0)
							$fileData[$l][2]=1;
					}
					//$fileData[$k][2]=1;
				}
			}
		}
	}
	for($i=0; $i<$tagLineCounter; $i++)
	{
		$fileData[$i][2]=0;
	}
	
	if(!$ifDataFound)
		echo '<div align="center" class="textstyle">No data found!</div>';
}
if($myfile)
fclose($myfile);

?>