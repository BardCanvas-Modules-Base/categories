<?
/**
 * Categories browser
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *             
 * @var string $_ROOT_URL
 */

use hng2_modules\categories\category_repository;
use hng2_tools\record_browser;

$browser    = new record_browser("{$config->website_key}_{$current_module->name}");
$repository = new category_repository();

/**
 * @var int    $offset
 * @var int    $limit
 * @var int    $order
 * @var string $search_state
 * @var string $search_for
 */
#region Nav filters
$built_vars = $browser->build_vars(20, 3);
foreach($built_vars as $key => $val) $$key = $val;

$where       = array();
$search_for  = empty($search_for) ? "" : trim($search_for);
if( ! empty($search_for) )
{
    if( strpos($search_for, ",") !== false )
    {
        $search_for_exploded = explode(",", $search_for);
        $where_line          = "";
        foreach( $search_for_exploded as $this_term )
        {
            $this_term = trim($this_term);
            if( ! empty($this_term) )
            {
                $where_line .= "id_category     =     '{$this_term}'  or ";
                $where_line .= "parent_category =     '{$this_term}'  or ";
                $where_line .= "slug            like '%{$this_term}%' or ";
                $where_line .= "title           like '%{$this_term}%' or ";
                $where_line .= "description     like '%{$this_term}%' or ";
            }
        }
        $where[] = substr($where_line, 0, -4);
    }
    else
    {
        $search_for = trim($search_for);
        $where[] 
            = "id_category     =     '{$search_for}'  or "
            . "parent_category =     '{$search_for}'  or "
            . "slug            like '%{$search_for}%' or "
            . "title           like '%{$search_for}%' or "
            . "description     like '%{$search_for}%'";
    }
}
#endregion

#region Nav pointers
$record_count = $repository->get_record_count($where);
$pagination   = $browser->build_pagination($record_count, $limit, $offset);
#endregion

#region Data grabbing
switch( $order )
{
    case  1: $sqlorder = "id_category asc,  parent_category asc";                       break;
    case  2: $sqlorder = "id_category desc, parent_category asc";                       break;
    case  3: $sqlorder = "title asc,        parent_category asc";                       break;
    case  4: $sqlorder = "title desc,       parent_category asc";                       break;
    case  5: $sqlorder = "visibility asc,   title asc,          parent_category asc";   break;
    case  6: $sqlorder = "visibility desc,  title desc,         parent_category asc";   break;
    case  7: $sqlorder = "min_level asc,    title asc,          parent_category asc";   break;
    case  8: $sqlorder = "min_level desc,   title desc,         parent_category asc";   break;
}

$records = $repository->find($where, $limit, $offset, $sqlorder);
#endregion
?>

<h1 class="clearfix">
    <button class="pull-right" onclick="$('#filter_form').submit()">
        <span class="fa fa-refresh"></span>
        <?= $language->refresh ?>
    </button>
    
    <?= $current_module->language->index->title; ?>
    
    <button onclick="prepare_category_addition()">
        <span class="fa fa-plus-circle"></span>
        <?= $current_module->language->index->buttons->add ?>
    </button>
</h1>

<div class="navcontrols clearfix">
    <form name="filter_form" id="filter_form" action="<?= $_SERVER["PHP_SELF"] ?>?wasuuup=<? echo mt_rand(1, 65535); ?>"
          method="get">
        
        <input type="hidden" name="mode"   value="set_filter">
        <input type="hidden" name="order"  value="<?= $order ?>">
        <input type="hidden" name="offset" value="0">
        
        <span style="float: right;">
            <?= $language->record_nav->page ?>
            <?= $pagination["this_page_number"]; ?>/<?= $pagination["total_pages"]; ?>
            (<?= $pagination["total_records"]; ?> <?= $language->record_nav->entries ?>)
        </span>
        
        <?= $language->record_nav->search ?>
        <input type="text" name="search_for"
               value="<?= htmlspecialchars($search_for) ?>" size="35"
               placeholder="<?= $current_module->language->record_nav->placeholder ?>">
        
        <?= $language->record_nav->show ?>
        <select name="limit">
            <? foreach(array(5, 10, 20, 30, 40, 50, 100, 200, 300, 400, 500) as $recs): ?>
                <option <? if($limit == $recs) echo "selected" ?>><?= $recs ?></option>
            <? endforeach; ?>
        </select>
        <?= $language->record_nav->recs_per_page ?>
        
        <button type="submit"><?= $language->record_nav->buttons->apply ?></button>
        <button type="submit" onclick="reset_filter();"><?= $language->record_nav->buttons->reset ?></button>
    </form>
</div>

<? if( $record_count == 0 ): ?>
    <div class="framed_content state_ko">
        <span class="fa fa-info-circle"></span>
        <?= $language->record_nav->no_records_found ?>
    </div>
    <? return; ?>
<? endif;  ?>

<div class="table_wrapper">
    <table class="nav_table evened">
        
        <thead>
        <tr>
            <? foreach( $browser->build_table_header($current_module->language->record_nav->columns) as $this_cell ): ?>
                <th align="<?= $this_cell->xalign ?>" <? if( $this_cell->xnowrap == true ) echo "nowrap "; ?>
                    <? if( ! empty($this_cell->xwidth) ) echo "width='$this_cell->xwidth' "; ?> class="<?= $this_cell->xclass ?>"><?
                    if( $this_cell->sort_asc->enabled == true ) echo "<img
                    src='{$_root_url}/media/icons/order_asc" . ($order == $this_cell->sort_asc->order ? "_this" : "") . ".gif' 
                    align='absmiddle' width='7' height='7'  style='cursor: pointer;'
                    alt='{$this_cell->sort_asc->alt}' title='{$this_cell->sort_asc->alt}'
                    onclick=\"document.filter_form.order.value='{$this_cell->sort_asc->order}'; document.filter_form.submit();\">&nbsp;";
                    ?><?= $this_cell->content ?><?
                    if( $this_cell->sort_desc->enabled == true ) echo "&nbsp;<img
                    src='{$_root_url}/media/icons/order_desc" . ($order == $this_cell->sort_desc->order ? "_this" : "") . ".gif' 
                    align='absmiddle' width='7' height='7'  style='cursor: pointer;'
                    alt='{$this_cell->sort_desc->alt}' title='{$this_cell->sort_desc->alt}'
                    onclick=\"document.filter_form.order.value='{$this_cell->sort_desc->order}'; document.filter_form.submit();\">";
                    ?></th>
            <? endforeach; ?>
            <th nowrap align="center"><?= $language->record_nav->actions ?></th>
        </tr>
        </thead>
    
        <tbody>
        <? foreach( $records as $row ): ?>
        
            <tr data-record-id="<?= $row->id_category ?>">
                <td>
                    <?= $row->title ?><br>
                    <i><?= $row->slug ?></i>
                </td>
                <td>
                    <?= $row->description ?>
                </td>
                <td>
                    <?= $row->visibility ?>
                </td>
                <td>
                    <?= $row->min_level ?>
                </td>
            </tr>
        <? endforeach; ?>
        </tbody>
    </table>
</div>

<div id="pagination">
    <? $browser->render_pagination_controls("paginate", $pagination); ?>
</div>
