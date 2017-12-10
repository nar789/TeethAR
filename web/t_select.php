<?
	include ("dblib.php");
	$conn=mysqli_connect($db_host,$db_user,$db_passwd,$db_name);
	$result=mysqli_query($conn,"select * from t_pos");
	$row=mysqli_fetch_array($result);
	echo "{\"w\":".$row['w'].",\"h\":".$row['h']."}";
	mysqli_close($conn);
?>