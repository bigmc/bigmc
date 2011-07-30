<?php
include('header.php');
$fname = basename($_GET['page']);

if(empty($fname)) {
	$fname = "index";
}

if(file_exists($fname . ".inc"))
	include($fname . ".inc");
else
	echo "<h2>File not found</h2>";

include('footer.php');
?>
