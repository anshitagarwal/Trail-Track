<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<link rel="stylesheet" href="dist/css/bootstrap.css">
<link rel="stylesheet" href="dist/css/bootstrap-glyphicons.css">
<script type="text/javascript" src="dist/css/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="dist/css/bootstrap.min.js"></script>
<style type="text/css">
body {
    background-image:    url(background.jpg);
    background-size:     cover; 
    background-repeat:   no-repeat;
    background-position: center center;
}
html { overflow-y: hidden; }
</style>

<title>Trail Track</title>
</head>
<body>
<form method="get" action="search.php">
  <table width="100%" height="100%" border="0">
    <tr align="center">
      <td align="center">
	  <input name="searchValue" type="text" id="searchValue" class="form-control" placeholder="Enter a Query"/>
	  <input name="searchTagValue" type="text" id="searchTagValue" class="form-control" placeholder="Enter a Query Tag"/>
	  <br>
	  <input name="SearchButton" type="submit" value="Track the Trail" class="btn btn-primary"/></td>
    </tr>
  </table>
</form>
<?php

?>
</body>
</html>
