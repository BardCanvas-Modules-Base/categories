<?php
/**
 * Category tree as json
 * Note: it is for generic usage! make sure to filter user levels accordignly.
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * $_GET params:
 * @param with_description
 * @param exclude_default
 */

use hng2_modules\categories\categories_repository;

include "../../config.php";
include "../../includes/bootstrap.inc";

header("Content-Type: application/json; charset=utf-8");

if( ! $account->_exists ) die(json_encode(array("message" => $language->errors->page_requires_login )));

$repository       = new categories_repository();
$with_description = $_GET["with_description"] == "true";

$main_category = $repository->get("0000000000000", true);
$tree_filter   = array("id_category <> '0000000000000'");
$data          = array("0000000000000" => $main_category->title);
$res           = $repository->get_as_tree_for_select($tree_filter, "", $with_description);

foreach($res as $key => $val) $data[$key] = $val;

echo json_encode(array("message" => "OK", "data" => $data));
