<html>
<body>
<form method="post" action="newacc.php">
<div style="text-align: center"><b>Tiny account maker for YurOTS on Rookgard</b></div>
<table cellspacing="0" width="100%">
 <tr><td width="20%">Account:</td>
  <td><?
$account = rand(100000, 999999);
print '' . $account . ' <input type="hidden" name="account" value="' . $account . '">' . "\n";
  ?></td>
 </tr>

 <tr>
  <td width="20%">Name:</td>
  <td><input type="text" name="nick" maxlength="20" /> <span class="copy">3-20 characters</span></td>
 </tr>
 <tr>
  <td width="20%">Password: </td>
  <td><input type="password" name="pass" maxlength="10"> <span>3-10 characters</span></td>
 <tr>

 <tr>
  <td width="20%">Sex:</td>
  <td>
   <input type="radio" name="sex" value="1" checked> Male
   <input type="radio" name="sex" value="0"> Female
  </td>
 </tr>

 <tr>
  <td colspan="2">
   <input type="submit" value="Create">&nbsp;
   <input type="reset" value="Reset">
  </td>
 </tr>
</table>
</form>
</div>
</body>
</html>
