<?php
/**
 * Category tree as json
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_modules\categories\category_repository;

header("Content-Type: application/json; charset=utf-8");
$_ROOT_URL = "../..";
include "{$_ROOT_URL}/config.php";
include "{$_ROOT_URL}/includes/bootstrap.inc";
session_start();

if( ! $account->_is_admin ) die(json_encode(array("message" => $language->errors->page_requires_login )));

$repository = new category_repository();
$data       = $repository->get_as_tree_for_select();

echo json_encode(array("message" => "OK", "data" => $data));
