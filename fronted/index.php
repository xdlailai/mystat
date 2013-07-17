<?php
    require 'config.php';
    require 'localize.php';
    require 'parsexml.php';

    validate_input();

    require "./themes/$style/theme.php";

    function write_side_bar()
    {
        global $iface, $page, $graph, $script, $style;
        global $iface_list, $iface_title;
        global $page_list, $page_title;

        $p = "&amp;graph=$graph&amp;style=$style";

        print "<ul class=\"iface\">\n";
        foreach ($iface_list as $if)
        {
            print "<li class=\"iface\">";
            if (isset($iface_title[$if]))
            {
                print $iface_title[$if];
            }
            else
            {
                print $if;
            }
            print "<ul class=\"page\">\n";
            foreach ($page_list as $pg)
            {
                print "<li class=\"page\"><a href=\"$script?if=$if$p&amp;page=$pg\">".$page_title[$pg]."</a></li>\n";
            }
            print "</ul></li>\n";

        }
        print "</ul>\n";
    }


    function kbytes_to_string($kb)
    {
        $units = array('TB','GB','MB','KB');
        $scale = 1024*1024*1024;
        $ui = 0;

        while (($kb < $scale) && ($scale > 1))
        {
            $ui++;
            $scale = $scale / 1024;
        }
        return sprintf("%0.2f %s", ($kb/$scale),$units[$ui]);
    }

    function write_summary()
    {
        global $top,$day,$month,$total_rx, $total_tx;
        global $cpu, $mem, $load, $download_rate, $upload_rate;
        global $partition_dir, $partition_total, $partition_used, $partition_pct;

        $trx = $total_rx;
        $ttx = $total_tx;

        //
        // build array for write_data_table
        //

        $sum[1]['act'] = 1;
        $sum[1]['label'] = T('This day');
        $sum[1]['rx'] = $day[0]['rx'];
        $sum[1]['tx'] = $day[0]['tx'];

        $sum[2]['act'] = 1;
        $sum[2]['label'] = T('This month');
        $sum[2]['rx'] = $month[0]['rx'];
        $sum[2]['tx'] = $month[0]['tx'];

        $sum[3]['act'] = 1;
        $sum[3]['label'] = T('All time');
        $sum[3]['rx'] = $trx;
        $sum[3]['tx'] = $ttx;

        write_data_table('Summary', $sum);


        print "<tr>";
        print "<td class=\"label\" style=\"width:120px;\">&nbsp;</td>";
        print "<td class=\"label\">".cpu利用率."</td>";
        print "<td class=\"label\"> $cpu </td>";
        print "</tr>\n";
        print "<tr>";
        print "<td class=\"label\" style=\"width:120px;\">&nbsp;</td>";
        print "<td class=\"label\">".内存使用率."</td>";
        print "<td class=\"label\"> $mem</td>";
        print "</tr>\n";
        print "<tr>";
        print "<td class=\"label\" style=\"width:120px;\">&nbsp;</td>";
        print "<td class=\"label\">".系统负载."</td>";
        print "<td class=\"label\"> $load </td>";
        print "</tr>\n";
        print "<tr>";
        print "<td class=\"label\" style=\"width:120px;\">&nbsp;</td>";
        print "<td class=\"label\">".下行速率KB."</td>";
        print "<td class=\"label\"> $download_rate </td>";
        print "</tr>\n";
        print "<tr>";
        print "<td class=\"label\" style=\"width:120px;\">&nbsp;</td>";
        print "<td class=\"label\">".上行速率KB."</td>";
        print "<td class=\"label\"> $upload_rate </td>";
        print "</tr>\n";
        print "<tr>";
        print "<td class=\"label\" style=\"width:120px;\">&nbsp;</td>";
        print "<td class=\"label\">".硬盘使用情况（）."</td>";
        print "<td class=\"label\"> $partition_total </td>";
        print "<td class=\"label\"> $partition_used </td>";
        print "<td class=\"label\"> $partition_pct </td>";
        print "</tr>\n";
        print "<br/>\n";
        #write_data_table('Top 10 days', $top);
    }


    function write_data_table($caption, $tab)
    {
        print "<table width=\"100%\" cellspacing=\"0\">\n";
        print "<caption>$caption</caption>\n";
        print "<tr>";
        print "<th class=\"label\" style=\"width:120px;\">&nbsp;</th>";
        print "<th class=\"label\">".T('In')."</th>";
        print "<th class=\"label\">".T('Out')."</th>";
        print "<th class=\"label\">".T('Total')."</th>";
        print "</tr>\n";

        for ($i=0; $i<count($tab); $i++)
        {
            if ($tab[$i]['act'] == 1)
            {
                $t = $tab[$i]['label'];
                $rx = kbytes_to_string($tab[$i]['rx']);
                $tx = kbytes_to_string($tab[$i]['tx']);
                $total = kbytes_to_string($tab[$i]['rx']+$tab[$i]['tx']);
                $id = ($i & 1) ? 'odd' : 'even';
                print "<tr>";
                print "<td class=\"label_$id\">$t</td>";
                print "<td class=\"numeric_$id\">$rx</td>";
                print "<td class=\"numeric_$id\">$tx</td>";
                print "<td class=\"numeric_$id\">$total</td>";
                print "</tr>\n";
             }
        }
        print "</table>\n";
    }

    get_xml_data();

    //
    // html start
    //
    header('Content-type: text/html; charset=utf-8');
    print '<?xml version="1.0"?>';
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <title>traffic counter</title>
  <link rel="stylesheet" type="text/css" href="themes/<?php echo $style ?>/style.css"/>
</head>
<body>

<div id="wrap">
  <div id="sidebar"><?php write_side_bar(); ?></div>
   <div id="content">
    <div id="header"><?php print T('Traffic data for')." $iface_title[$iface] ($iface)";?></div>
    <div id="main">
    <?php
    $graph_params = "if=$iface&amp;page=$page&amp;style=$style";
    if ($page != 's')
        if ($graph_format == 'svg') {
	     print "<object type=\"image/svg+xml\" width=\"692\" height=\"297\" data=\"graph_svg.php?$graph_params\"></object>\n";
        } else {
	     print "<img src=\"graph.php?$graph_params\" alt=\"graph\"/>\n";
        }

    if ($page == 's')
    {
        write_summary();
    }
    else if ($page == 'd')
    {
        write_data_table(T('Last 30 days'), $day);
    }
    else if ($page == 'm')
    {
        write_data_table(T('Last 12 months'), $month);
    }
    ?>
    </div>
    <div id="footer"><a href="http://www.sqweek.com/">new counter</a> 1.5.1 - &copy;2013 fly100</div>
  </div>
</div>

</body></html>
