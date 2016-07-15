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

foreach($listing as $row)
    $lines[] = "<a class='item' href='{$config->full_root_url}/category/{$row->slug}/'>{$row->title}</a>";

$lines[] = "</div>";

$template->add_left_sidebar_group("", implode("\n", $lines), 0);
