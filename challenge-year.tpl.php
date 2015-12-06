<?php
// $Id$
?>

<div class="challenge">

<?php if ($variables[challenges]) {?>
<?php foreach($variables[challenges] as $list) {?>
<h3><?php echo ucfirst($list[0]->category); ?></h3>

<ul>
  <?php foreach($list as $item) {?>
  <li><a href="<?php echo '/challenge/'.$item->year.'/'.$item->category.'/'.$item->ord?>"><?php echo $item->title;?></a><li>
  <?php }?>
</ul>

<?php }?>
<?php } else echo 'no this page!';?>

</div>
