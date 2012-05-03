<?php
// $Id$

/*
 * The function is used to list different categories.
 */
function challenge_category_list($year, $category){
  global $user;

  $result = db_query("SELECT n.nid, n.title, v.body, c.year, c.category, c.ord, c.passed, c.submitted, c.score FROM {challenge} c INNER JOIN {node} n ON n.nid=c.nid INNER JOIN {node_revisions} v ON c.nid=v.nid WHERE n.status=1 AND c.year=%d AND c.category='%s' ORDER BY c.ord ASC", $year, $category);
  while ( $row = db_fetch_object($result)){
    $list[] = $row;
  }
  if (!$list)
    return t('No this challenge!');
  $result = db_query("SELECT passed FROM {users} WHERE uid=$user->uid");
  $passed = explode(',', db_fetch_object($result)->passed);
  
  drupal_set_breadcrumb(array(
        l(t('Home'), NULL),
        l(t('Challenge'), 'challenge'),
        l(t('!year', array('!year' => $year)), 'challenge/'.$year),
        l(t('!category', array('!category' => $category)), 'challenge/'.$year.'/'.$category),
      )
    );

  return theme('challenge_category_list', $list, $passed); 
}

/*
 * Challenge index display.
 */
function challenge_index(){
  return theme('challenge_index','this is challenge index');
}

/*
 * Challenge Year display.
 */
function challenge_year($year){
  $challenges['basic']   = array();
  $challenges['script']  = array();
  $challenges['crack']   = array();
  $challenges['kernel']  = array();
  $challenges['reality'] = array();
  $result = db_query("SELECT n.nid, n.title, c.year, c.category, c.ord FROM {challenge} c INNER JOIN {node} n ON n.nid=c.nid WHERE n.status=1 AND c.year=%d ORDER BY c.category, c.ord", $year);
  while ( $row = db_fetch_object($result)){
    $challenges[$row->category][] = $row;
  }
  if (!$challenges)
    return 'No challenges!';
  
  drupal_set_breadcrumb(array(
        l(t('Home'), NULL),
        l(t('Challenge'), 'challenge'),
        l(t('!year', array('!year' => $year)), 'challenge/'.$year),
      )
    );
  return theme('challenge_year', $challenges); 
}

/*
 * Challenge display page
 */
function challenge_display($year, $category, $ord){
  $result = db_query("SELECT n.nid, n.title, v.body, v.format, c.hint, c.year, c.category, c.ord, c.score, c.file, c.viewed, c.submitted, c.passed, c.judgetype, c.dynamic,  c.judger FROM {challenge} c INNER JOIN {node} n ON n.nid=c.nid INNER JOIN {node_revisions} v ON c.nid=v.nid WHERE n.status=1 AND c.year=%d AND c.category='%s' AND c.ord=%d", $year, $category, $ord);

  $item = db_fetch_object($result);
  if (!$item)
    return 'No challenge!';

  $item = node_prepare($item, $teaser = FALSE);
  drupal_set_breadcrumb(array(
        l(t('Home'), NULL),
        l(t('Challenge'), 'challenge'),
        l(t('!year', array('!year' => $item->year)), 'challenge/'.$item->year),
        l(t('!category', array('!category' => $item->category)), 'challenge/'.$item->year.'/'.$item->category),
        l(t('!ord', array('!ord' => $item->ord)), "challenge/$item->year/$item->category/$item->ord"),
      )
  );
  return theme('challenge_display', $item);
}

function challenge_manual_judge($status=0){
  global $user;
  
  drupal_set_breadcrumb(array(
      l(t('Home'), NULL),
      l(t('Challenge'), 'challenge'),
      l(t('Manual Judge'), 'challenge/manual_judge'),
    )
  );

  $header = array(
    array('data' => t('User')),
    array('data' => t('Challenge Name')),
    array('data' => t('File')),
    array('data' => t('Submit Time')),
    array('data' => t('Operation')),
  );
  $query = "SELECT m.uid, m.uname, m.nid, m.chname, m.judger, m.file, m.timestamp, m.status FROM {manual_judge} m WHERE m.status=$status ORDER BY m.nid, m.timestamp";
  $result = pager_query($query, 20);
  while ($row = db_fetch_object($result)){
    if ( in_array($user->name, explode(',', $row->judger)) || in_array('Site Administrator', $user->roles)){
      $operation = '<a href="javascript:'."judge_confirm('$row->uname', '$row->chname', 'approve', '/challenge/manual_judge/judge/$row->nid/$row->uid/1/')".'"> Approve </a>';
      $operation .= '<a href="javascript:'."judge_confirm('$row->uname', '$row->chname', 'deny','/challenge/manual_judge/judge/$row->nid/$row->uid/2/')".'"> Deny </a>';
      $rows[] = array(
        array('data' => l($row->uname, 'user/'.$row->uid)),
        array('data' => $row->chname),
        array('data' => l('Download', $row->file)),
        array('data' => date("Y-m-d H:i:s",$row->timestamp)),
        array('data' => $operation),
      );
    }
  }
  $content = theme('table', $header, $rows);
  $content.= theme('pager');

  $content.= <<<HTML
<script type="text/javascript">
function judge_confirm(user, challenge, operate, dest){
  if (confirm("Confirm to "+operate+" "+user+"'s solution for "+challenge+"?" ))
    window.location=dest;
}
</script>
HTML;
  
  return $content;

  //return theme('challenge_manual_judge', $list);
}

/*
 * Challenge Pass log
 */
function challenge_passlog($uid=0){
  drupal_set_breadcrumb(array(
      l(t('Home'), NULL),
      l(t('Challenge'), 'challenge'),
      l(t('Passlog'), 'challenge/passlog'),
    )
  );
  $header = array(
    array('data' => t('User')),
    array('data' => t('Challenge Name')),
    array('data' => t('Pass Time')),
  );
  if($uid){
    $query = "SELECT * FROM {passlog} WHERE uid=$uid ORDER BY time DESC";
  }
  else{
    $query = "SELECT * FROM {passlog} ORDER BY time DESC";
  }
  $result = pager_query($query, 20);
  while ($row = db_fetch_object($result)){
    $rows[] = array(
      array('data' => l($row->uname, 'user/'.$row->uid)),
      array('data' => $row->chname),
      array('data' => date("Y-m-d H:i:s",$row->time)),
    );
  }
  $content = theme('table', $header, $rows);
  $content.= theme('pager');
  return $content;
}

/*
 * Ranking Page
 */
function challenge_ranking(){
  drupal_set_breadcrumb(array(
      l(t('Home'), NULL),
      l(t('Challenge'), 'challenge'),
      l(t('Ranking'), 'challenge/ranking'),
    )
  );

  $limit = 20;
  $page = isset($_GET['page']) ? ($_GET['page']) : 0;
  $rank = $limit * $page + 1;
  $header = array(
    array('data' => t('Rank')),
    array('data' => t('User')),
    array('data' => t('Score')),
  );
  $query = "SELECT * FROM {users} ORDER BY score DESC";
  $result = pager_query($query, $limit);
  while ($row = db_fetch_object($result)){
    if (!$row->uid) continue;
    $rows[] = array(
      array('data' => $rank),
      array('data' => $row->name),
      array('data' => $row->score),
    );  
    $rank ++;
  }
  $content = theme('table', $header, $rows);
  $content.= theme('pager');
  return $content;
}

