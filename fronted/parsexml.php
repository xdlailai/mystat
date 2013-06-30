<?php


$page_list = array('s', 'd', 'm');
$graph_list = array('large', 'small', 'none');
$page_title['s'] = 'summary';
$page_title['d'] = 'days';
$page_title['m'] = 'months';

function validate_input()
{
  global $page, $page_list;
  global $iface, $iface_list;
  global $graph, $graph_list;
  global $colorscheme, $style;

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  $iface = isset($_GET['if']) ? $_GET['if'] : '';
  $graph = isset($_GET['graph']) ? $_GET['graph'] : '';
  $style = isset($_GET['style']) ? $_GET['style'] : '';
  if(!in_array($page, $page_list))
  {
    $page = $page_list[0];
  }
  if(!in_array($iface, $iface_list))
  {
    $iface = $iface_list[0];
  }
  if(!in_array($graph, $graph_list))
  {
    $graph = $graph_list[0];
  }
  $tp = "./themes/$style";
  if(!is_dir($tp) || !file_exists("$tp/theme.php"))
  {
    $style = DEFAULT_COLORSCHEME;
  }
}

function get_xml_data()
{
  global $day, $month, $top, $interface, $total_rx, $total_tx;
  $xml=simplexml_load_file("dataxml");
  $interface= $xml->id;
  $created_year=$xml->created->date->year;
  $created_month=$xml->created->date->month;
  $created_day=$xml->created->date->day;
  $updated_year=$xml->updated->date->year;
  $updated_month=$xml->updated->date->month;
  $updated_day=$xml->updated->date->day;
  $updated_hour=$xml->updated->time->hour;
  $updated_minute=$xml->updated->time->minute;
  $total_rx=$xml->traffic->total->rx;
  $total_tx=$xml->traffic->total->tx;
  $day = array();
  $month = array();
  $top = array();
  foreach($xml->traffic->days->day as $myday){
    $id=$myday['id'];
    $day[trim($id)]['time'] = $myday->date->year."-".$myday->date->month."-".$myday->date->day;
    $day[trim($id)]['rx'] = $myday->rx;
    $day[trim($id)]['tx'] = $myday->tx;
    $day[trim($id)]['label'] = $myday->date->month."-".$myday->date->day;
    $day[trim($id)]['img_label'] = $myday->date->day;
  }
  foreach($xml->traffic->months->month as $mymonth){
    $id=$mymonth['id'];
    $month[trim($id)]['time'] = $mymonth->date->year."-".$mymonth->date->month;
    $month[trim($id)]['rx'] = $mymonth->rx;
    $month[trim($id)]['tx'] = $mymonth->tx;
    $month[trim($id)]['label'] = $mymonth->date->year."-".$mymonth->date->month;
    $month[trim($id)]['img_label'] = $mymonth->date->month;
  }
  foreach($xml->traffic->tops->top as $mytop){
    $id=$mytop['id'];
  }
}
?>
