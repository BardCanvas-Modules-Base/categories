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
 * @return string
 */

use hng2_base\module;
use hng2_base\template;
use hng2_modules\categories\categories_repository;

global $config, $language;

$repository = new categories_repository();
$listing    = $repository->get_for_listings();
$lines      = array();

if( empty($listing) ) return "";

foreach($listing as $row)
{
    $added_class = $template->get("current_category_slug") == $row->slug ? "selected" : "";
    $added_class .= $row->visibility == "public" ? "" : " important";
    $lines[] = "<a class='item $added_class' href='{$config->full_root_url}/category/{$row->slug}/'>{$row->title}</a>";
}

return implode("\n", $lines);
