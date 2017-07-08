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

include "../../config.php";
include "../../includes/bootstrap.inc";

header("Content-Type: application/json; charset=utf-8");

if( ! $account->has_admin_rights_to_module("categories") )
    die(json_encode(array("message" => trim($language->errors->access_denied) )));

if( empty($_GET["id_category"]) ) die(json_encode(array("message" => trim($current_module->language->messages->missing->id) )));

$repository = new categories_repository();
$record     = $repository->get($_GET["id_category"], true);

if( is_null($record) ) die(json_encode(array("message" => trim($current_module->language->messages->category_not_found) )));

$data = $record->get_as_associative_array();
$config->globals["modules:categories.json_record"] =& $data;
$current_module->load_extensions("get_as_json", "before_outputting_data");

echo json_encode(array("message" => "OK", "data" => $data));
