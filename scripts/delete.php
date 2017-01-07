<?php
/**
 * Category deleter
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
if( ! $account->has_admin_rights_to_module("categories") ) throw_fake_401();

if( empty($_GET["id_category"]) ) die($current_module->language->messages->missing->id);

$repository = new categories_repository();
$count      = $repository->get_record_count(array("id_category" => $_GET["id_category"]));

if( $count == 0 ) die($current_module->language->messages->category_not_found);

if( $_GET["id_category"] == "0000000000000" ) die($current_module->language->messages->cannot_delete_default);;

$deleted = $repository->delete($_GET["id_category"]);
$repository->purge_caches();

send_notification($account->id_account, "success", replace_escaped_vars(
    $current_module->language->messages->category_deleted,
    array('{$name}', '{$children}', '{$items}'),
    array($_GET["id_category"], $deleted, 0)
));

echo "OK";
