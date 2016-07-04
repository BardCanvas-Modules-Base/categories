<?php
/**
 * Categories browser
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

$_ROOT_URL = "..";
include "{$_ROOT_URL}/config.php";
include "{$_ROOT_URL}/includes/bootstrap.inc";
if( ! $account->_is_admin ) throw_fake_404();
session_start();

$template->page_contents_include = "contents/browser.inc";
$template->set_page_title($current_module->language->index->title);
include "{$template->abspath}/embeddable.php";
