<?
/**
 * Categories browser
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_modules\categories\categories_repository;
use hng2_tools\record_browser;

$browser    = new record_browser("{$config->website_key}_{$current_module->name}");
$repository = new categories_repository();

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
    case  1: $sqlorder = "parent_category asc,  title asc";                         break;
    case  2: $sqlorder = "parent_category desc, title desc";                        break;
    case  3: $sqlorder = "title asc";                                               break;
    case  4: $sqlorder = "title desc";                                              break;
    case  5: $sqlorder = "visibility asc,      parent_category asc, title asc  ";   break;
    case  6: $sqlorder = "visibility desc,     parent_category asc, title desc ";   break;
    case  7: $sqlorder = "min_level asc,       parent_category asc, title asc  ";   break;
    case  8: $sqlorder = "min_level desc,      parent_category asc, title desc ";   break;
}

$config->globals["categories:nav/presettings_data"] = array($sqlorder, array());
$current_module->load_extensions("browser", "presettings_data");
list($sqlorder, $additional_select_columns) = $config->globals["categories:nav/presettings_data"];
unset( $config->globals["categories:nav/presettings_data"] );
if( ! empty($additional_select_columns) ) $repository->add_select_fields($additional_select_columns);

$records = $repository->find($where, $limit, $offset, $sqlorder);
$tree    = $repository->get_slug_paths();

$per_column_function_hooks = array();
$current_module->load_extensions("browser", "per_record_function_hooks");

#endregion
?>

<h1 class="clearfix">
    <button id="refresh_category_browser" class="pull-right" onclick="$('#filter_form').submit()">
        <span class="fa fa-refresh"></span>
        <?= $language->refresh ?>
    </button>
    
    <?= $current_module->language->index->title; ?>
    
    <button onclick="prepare_category_addition()">
        <span class="fa fa-plus-circle"></span>
        <?= $current_module->language->index->buttons->add ?>
    </button>
</h1>

<div class="filtering clearfix">
    
    <script type="text/javascript">
        $(document).ready(function()
        {
            $('#filter_form').keypress(function(e) {
                if( (e.keyCode == 13) && (e.target.type != "textarea") )
                {
                    e.preventDefault();
                    $(this).submit();
                }
            });
        });
    </script>
    
    <form name="filter_form" id="filter_form" action="<?= $_SERVER["PHP_SELF"] ?>?wasuuup=<? echo mt_rand(1, 65535); ?>"
          method="get">
        
        <input type="hidden" name="mode"   value="set_filter">
        <input type="hidden" name="order"  value="<?= $order ?>">
        <input type="hidden" name="offset" value="0">
        
        <span style="float: right;">
            <?= $browser->get_pagination_button("previous", "paginate", $pagination) ?>
            <?= $language->record_nav->page ?>
            <?= $pagination["this_page_number"]; ?>/<?= $pagination["total_pages"]; ?>
            (<?= $pagination["total_records"]; ?> <?= $language->record_nav->entries ?>)
            <?= $browser->get_pagination_button("next", "paginate", $pagination) ?>
        </span>
        
        <?= $language->record_nav->search ?>
        <input type="text" name="search_for"
               value="<?= htmlspecialchars($search_for) ?>" size="35"
               placeholder="<?= $current_module->language->record_nav->placeholder ?>">
        
        <?= $language->record_nav->show ?>
        <select name="limit" onchange="$('#filter_form').submit()">
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
    <table id="categories_browser_table" class="nav_table evened">
        
        <thead>
        <tr>
            <?
            $config->globals["categories:nav/additional_headers"]
                = $browser->build_table_header($current_module->language->record_nav->columns);
            $current_module->load_extensions("browser", "additional_headers");
            $headers = $config->globals["categories:nav/additional_headers"];
            unset( $config->globals["categories:nav/additional_headers"] );
            
            foreach( $headers as $this_cell ): ?>
                <th align="<?= $this_cell->xalign ?>" <? if( $this_cell->xnowrap == true ) echo "nowrap "; ?>
                    <? if( ! empty($this_cell->xwidth) ) echo "width='$this_cell->xwidth' "; ?> class="<?= $this_cell->xclass ?>"><?
                    if( $this_cell->sort_asc->enabled == true ) echo "<img
                    src='{$config->full_root_path}/media/icons/order_asc" . ($order == $this_cell->sort_asc->order ? "_this" : "") . ".gif' 
                    align='absmiddle' width='7' height='7'  style='cursor: pointer;'
                    alt='{$this_cell->sort_asc->alt}' title='{$this_cell->sort_asc->alt}'
                    onclick=\"document.filter_form.order.value='{$this_cell->sort_asc->order}'; $('#filter_form').submit();\">&nbsp;";
                    ?><?= $this_cell->content ?><?
                    if( $this_cell->sort_desc->enabled == true ) echo "&nbsp;<img
                    src='{$config->full_root_path}/media/icons/order_desc" . ($order == $this_cell->sort_desc->order ? "_this" : "") . ".gif' 
                    align='absmiddle' width='7' height='7'  style='cursor: pointer;'
                    alt='{$this_cell->sort_desc->alt}' title='{$this_cell->sort_desc->alt}'
                    onclick=\"document.filter_form.order.value='{$this_cell->sort_desc->order}'; $('#filter_form').submit();\">";
                    ?></th>
            <? endforeach; ?>
        </tr>
        </thead>
        
        <tbody>
        <? foreach( $records as $row ): ?>
            <tr data-record-id="<?= $row->id_category ?>" onclick="$(this).toggleClass('clicked')">
                
                <!-- Path -->
                <td>
                    <?
                    $path  = $tree[$row->id_category];
                    $parts = explode("/", $path);
                    array_pop($parts);
                    if( count($parts) <= 1 )
                    {
                        echo "―";
                    }
                    else
                    {
                        foreach($parts as $index => $part)
                        {
                            if( $index == 0 ) continue;
                            
                            if( $index == 1 )
                                echo "<span class='inline-block nowrap'>$part</span>";
                            else
                                echo "<span class='inline-block nowrap'><span class='fa fa-angle-right fa-fw'></span>$part</span>";
                        }
                    }
                    
                    if( ! empty($per_column_function_hooks["parent"]) )
                        foreach($per_column_function_hooks["parent"] as $function)
                            if( function_exists($function) ) $function($row);
                    ?>
                </td>
                
                <!-- Details -->
                <td width="40%">
                    
                    <?
                    if( ! empty($per_column_function_hooks["crucial_details"]) )
                        foreach($per_column_function_hooks["crucial_details"] as $function)
                            if( function_exists($function) ) $function($row);
                    ?>
                    
                    <div class="principal_data"><?= $row->title ?> [<code><?= $row->id_category ?></code>]</div>
                    
                    <?
                    if( ! empty($per_column_function_hooks["important_details"]) )
                        foreach($per_column_function_hooks["important_details"] as $function)
                            if( function_exists($function) ) $function($row);
                    ?>
                    
                    <div class="actions">
                        <span class="action pseudo_link" onclick="edit_category('<?= $row->id_category ?>')">
                            <span class="fa fa-pencil"></span>
                            <?= $current_module->language->record_nav->actions->edit ?>
                        </span>
                        <span class="action pseudo_link" onclick="copy_category('<?= $row->id_category ?>')">
                            <span class="fa fa-copy"></span>
                            <?= $current_module->language->record_nav->actions->copy ?>
                        </span>
                        <? if($row->id_category != "0000000000000"): ?>
                            <span class="action pseudo_link" onclick="delete_category('<?= $row->id_category ?>')">
                                <span class="fa fa-trash"></span>
                                <?= $current_module->language->record_nav->actions->delete ?>
                            </span>
                        <? endif; ?>
                        
                        <?
                        if( ! empty($per_column_function_hooks["actions"]) )
                            foreach($per_column_function_hooks["actions"] as $function)
                                if( function_exists($function) ) $function($row);
                        ?>
                    </div>
                </td>
                
                <!-- Slug -->
                <td width="10%">
                    <span class="pseudo_link clipboard-copy" data-clipboard-text="<?= $row->slug ?>"
                        ><?= $row->slug ?> <span class="fa fa-clipboard"></span></span>
                    <?
                    if( ! empty($per_column_function_hooks["slug"]) )
                        foreach($per_column_function_hooks["slug"] as $function)
                            if( function_exists($function) ) $function($row);
                    ?>
                </td>
                
                <!-- Description -->
                <td width="50%">
                    <?
                    if( empty($row->description) ) echo $current_module->language->record_nav->columns->description->empty;
                    else                           echo $row->description;
                    
                    if( ! empty($per_column_function_hooks["description"]) )
                        foreach($per_column_function_hooks["description"] as $function)
                            if( function_exists($function) ) $function($row);
                    ?>
                </td>
                
                <!-- Other -->
                <td align="center"><?= $row->visibility ?></td>
                <td align="center"><?= $row->min_level ?></td>
                
                <!-- Additional columns -->
                <? $current_module->load_extensions("browser", "additional_columns"); ?>
            </tr>
        <? endforeach; ?>
        </tbody>
    </table>
</div>

<div class="pagination">
    <? $browser->render_pagination_controls("paginate", $pagination); ?>
</div>
