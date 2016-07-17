<?
/**
 * Categories left sidebar link groups builder
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_modules\categories\categories_repository;

$repository = new categories_repository();
$listing    = $repository->get_for_listings();

$lines = array();
$lines[] = "<div class='group'>";
$lines[] = "<div class='title'>{$this_module->language->left_sidebar->group_caption}</div>";

foreach($listing as $row)
{
    $added_class = $template->get("current_category_slug") == $row->slug ? "selected" : "";
    $lines[] = "<a class='item $added_class' href='{$config->full_root_url}/category/{$row->slug}/'>{$row->title}</a>";
}

$lines[] = "</div>";

$template->add_left_sidebar_group("", implode("\n", $lines), 0);
