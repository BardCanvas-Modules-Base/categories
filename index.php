<?php
/**
 * Categories index
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

include "../config.php";
include "../includes/bootstrap.inc";
if( ! $account->has_admin_rights_to_module("categories") ) throw_fake_401();

$template->page_contents_include = "contents/index.inc";
$template->set_page_title($current_module->language->index->title);
include "{$template->abspath}/admin.php";
