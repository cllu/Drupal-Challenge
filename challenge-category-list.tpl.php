<?php
// $Id$
?>
<div class="challenge">
<?php if($variables['list']) {?>

<h3><?php 
  $list = $variables['list']; 
  $passed = $variables['passed']; 
  echo 'ISCC '.$list[0]->year.' '.ucfirst($list[0]->category); 
?></h3>

<?php foreach($list as $item) {?>
<div style="width:600px; margin:20px; auto;">
  <div class="tableheader" style="">
    <span class="status"><?php if(in_array($item->nid, $passed)) echo t('Passed'); else echo t('Not passed');?></span>
    <span class="tabletitle"><?php echo "$item->title(".ucfirst($item->category)." $item->ord)";?></span>
  </div>
  <table class="challengetable">
    <tr>
      <td class="table-light" width="20%">Score</td>
      <td class="table-slight" width="30%"><?php echo $item->score?></td>
      <td class="table-light" width="20%">Difficulty</td>
      <td class="table-slight" width="30"></td>
    </tr>
    <tr>
      <td class="table-light">Passed/Submitted</td>
      <td class="table-slight"><?php echo $item->passed.'/'.$item->submitted;?></td>
      <td class="table-light">Entry</td>
      <td class="table-slight"><a href="<?php echo '/challenge/'.$item->year.'/'.$item->category.'/'.$item->ord?>">Cick Here</a></td>
    </tr>
  </table>
</div>
<?php }?>

<?php } else echo 'No challenge!'?>
</div>
