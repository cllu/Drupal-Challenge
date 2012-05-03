<?php
// $Id
/*
 * This page 
 */
?>

<?php if($list = $variables['list']) {?>
<ul>
<?php foreach ($variables['list'] as $item) {?>
  <li><?php echo $item->uname.' '.$item->chname?></li>
<?php }?>
</ul>
<?php } else {?>
No challenge to disposal
<?php }?>
