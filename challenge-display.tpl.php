<?php
// $Id$
?>
<div class="challenge">
<?php $item = $variables['item'];?>
<center>
  <h2><?php echo $item->title;?></h2>
  Category: <?php echo ucfirst($item->category).'->'.$item->ord;?>
  Passed/Submitted: <?php echo $item->passed.'/'.$item->submitted;?>
  Score: <?php echo $item->score?>
  <?php if(user_access('edit any challenge', $account)||user_access('edit own challenge', $account)) {?><a href="/node/<?php echo $item->nid?>/edit">Edit</a><?php }?> 
</center>
<br />
<?php echo $item->body;?>

<div style="text-align:center;">
<?php echo drupal_get_form('challenge_solution_form', $item);?>
</div>

<?php if ($item->hint != '') {?>
<div id="hint" style="margin-top:55px;text-align:center;">
  <input type="button" id="disp" value="Display Tips" onclick="javascript:showtips('disp', 'tips')">
  <div id="tips" style="text-align:center;margin-top:5px;display:none;">
    <?php echo $item->hint;?>
  </div>
</div>
<script type="text/javascript">
function showtips(button, tipdiv){
  if ($('#'+tipdiv).css('display') == "none") {
   //$('#'+tipdiv).style.display = "";
    $('#'+tipdiv).css('display', "");
    $('#'+button).val("Hide Tips");
  } else {
    $('#'+tipdiv).css('display', "none");
    $('#'+button).val("Display Tips");
  }
}
</script>
<?php }?>
<?php if($item->file != '') { ?><p>Related File: <a href="<?php echo file_create_url($item->file);?>">Download</a></p><?php }?>

</div>
