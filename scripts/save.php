<?php
/**
 * Category saver
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_modules\categories\category_record;
use hng2_modules\categories\categories_repository;

include "../../config.php";
include "../../includes/bootstrap.inc";
if( ! $account->has_admin_rights_to_module("categories") ) throw_fake_401();

$current_module->load_extensions("save", "before_validations");

if( empty($_POST["title"]) )       die($current_module->language->messages->missing->title);
if( empty($_POST["slug"]) )        die($current_module->language->messages->missing->slug);
if( empty($_POST["visibility"]) )  die($current_module->language->messages->missing->visibility);

if( ! in_array($_POST["visibility"], array("public", "users", "level_based")))
    die($current_module->language->messages->invalid->visibility);

if( $_POST["visibility"] == "level_based" && ! is_numeric($_POST["min_level"]) )
    die($current_module->language->messages->invalid->min_level);

if( $_POST["visibility"] == "level_based" && $_POST["min_level"] < 0 && $_POST["min_level"] > 255 )
    die($current_module->language->messages->invalid->min_level);

$repository = new categories_repository();

if( ! empty($_POST["parent_category"]) )
{
    $count = $repository->get_record_count(array("id_category" => $_POST["parent_category"]));
    if( $count == 0 ) die($current_module->language->messages->invalid->parent_category);
    
    if( ! empty($_POST["id_category"]) && $_POST["id_category"] == $_POST["parent_category"] )
        die($current_module->language->messages->self_parenting_not_allowed);
}

if( ! preg_match('/^[a-z0-9\-_]+$/', $_POST["slug"]) )
    die($current_module->language->messages->invalid->slug);

$count = $repository->get_record_count(array("id_category <> '{$_POST["id_category"]}'", "slug" => $_POST["slug"]));
if( $count > 0 ) die($current_module->language->messages->slug_already_used);

$category = new category_record();
$category->set_from_post();
if( $category->visibility != "level_based" ) $category->min_level = 0;

if( empty($_POST["id_category"]) ) $category->set_new_id();

$current_module->load_extensions("save", "before_saving");
$repository->save($category);
$current_module->load_extensions("save", "after_saving");

echo "OK";
