<?
	include ("dblib.php");
	$w=$_POST['w'];
	$h=$_POST['h'];
	$conn=mysqli_connect($db_host,$db_user,$db_passwd,$db_name);
	if(!$conn)echo "error!";
	$result=mysqli_query($conn,"update t_pos set w=$w,h=$h");
	mysqli_close($conn);
?>