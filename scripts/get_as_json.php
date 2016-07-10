<?php
/**
 * Category record as json
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * $_GET params
 * @param string "id_category"
 */

use hng2_modules\categories\categories_repository;

$_ROOT_URL = "../..";
include "{$_ROOT_URL}/config.php";
include "{$_ROOT_URL}/includes/bootstrap.inc";

header("Content-Type: application/json; charset=utf-8");

if( ! $account->_is_admin ) die(json_encode(array("message" => $language->errors->page_requires_login )));

if( empty($_GET["id_category"]) ) die(json_encode(array("message" => $current_module->language->messages->missing->id )));

$repository = new categories_repository();
$record = $repository->get($_GET["id_category"]);

if( is_null($record) ) die(json_encode(array("message" => $current_module->language->messages->category_not_found )));

echo json_encode(array("message" => "OK", "data" => $record->get_as_associative_array()));
