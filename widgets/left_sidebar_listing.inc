<?php
/**
 * Categories group for left sidebar
 * Called as include() from within a function
 *
 * @package    HNG2
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var module[]          $modules
 * @var module            $this_module    widgets_manager
 * @var template          $template       The caller
 * @var \SimpleXMLElement $widget
 * @var string            $seed
 * @var string            $data_key
 * 
 * @var toolbox           $toolbox
 * 
 * @return string
 */

use hng2_base\module;
use hng2_base\template;
use hng2_modules\categories\categories_repository;
use hng2_modules\widgets_manager\toolbox;

global $config, $language;

if( empty($config->globals["widgets_manager:toolbox"]) )
    $config->globals["widgets_manager:toolbox"] = new toolbox();

$toolbox       = $config->globals["widgets_manager:toolbox"];
$placed_widget = $toolbox->get_widget_from_data_key($data_key);
if( is_null($placed_widget) ) return "";

$data    = $toolbox->load_widget_data($placed_widget);
$ttl     = (empty($data["cache_ttl"])    ? 0 : $data["cache_ttl"]) * 3600;
$version = empty($data["cache_version"]) ? 1 : $data["cache_version"];

$repository = new categories_repository();
$listing    = $repository->get_for_listings($ttl, $version, "parent_category, title", true);
$lines      = array();

if( empty($listing) ) return "";

$reordered = array();
foreach($listing as $row) $reordered[$row->path] = $row;
ksort($reordered);
$is_tree = false;
foreach($reordered as $path => $category)
{
    if( strpos($path, "/", 1) !== false )
    {
        $is_tree = true;
        
        break;
    }
}
if( $is_tree) $listing = $reordered;

foreach($listing as $row)
{
    if( $data["public_categories_only"] && $row->visibility != "public" ) continue;
    
    $level       = count(explode("/", $row->path)) - 2;
    if( $data["max_branch_level"] > 0 && $level > $data["max_branch_level"] ) continue;
    
    $indent      = $level * 10;
    $opener      = empty($level) ? "" : "<span style='display: inline-block; margin-left: {$indent}px;'>";
    $closer      = empty($level) ? "" : "</span>";
    $added_class = $template->get("current_category_slug") == $row->slug ? "selected" : "";
    $added_class .= $row->visibility == "public" ? "" : " important";
    $lines[] = "<a class='item $added_class' data-level='$level' href='{$config->full_root_url}/category/{$row->slug}/'
                   >{$opener}{$row->title}{$closer}</a>";
}

return implode("\n", $lines);
