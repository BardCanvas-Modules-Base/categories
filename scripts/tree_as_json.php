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

$tree_filter = $account->_is_admin == false ? array() : array(
    $_GET["exclude_default"] == "true" ? "id_category <> '0000000000000'" : "true",
    "( visibility = 'public' or visibility = 'users' or (visibility  = 'level_based' and '{$account->level}' >= min_level) )"
);

$data = $repository->get_as_tree_for_select($tree_filter, "", $with_description);

echo json_encode(array("message" => "OK", "data" => $data));
