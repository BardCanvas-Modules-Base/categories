<?php
/**
 * Module installation file
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var string $module_install_action install, uninstall, enable, disable
 * @var array  $messages
 * @var array  $errors
 * @var module $this_module
 * @var module $current_module
 */

use hng2_base\module;

if( $module_install_action == "install" )
{
    $query = "
        CREATE TABLE IF NOT EXISTS categories (
            
            id_category       VARCHAR(32) NOT NULL DEFAULT '',
            parent_category   VARCHAR(32) NOT NULL DEFAULT '',
            
            slug              VARCHAR(128) NOT NULL DEFAULT '',
            title             VARCHAR(255) NOT NULL DEFAULT '',
            description       VARCHAR(255) NOT NULL DEFAULT '',
            
            visibility        ENUM('public', 'users', 'level_based') NOT NULL DEFAULT 'public',
            min_level         tinyint unsigned not null default 0,
            
            PRIMARY KEY           ( id_category ),
            INDEX   category_tree ( id_category, parent_category ),
            INDEX   by_slug       ( slug(5) )
            
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE='utf8mb4_unicode_ci'
    ";
    
    try
    {
        $database->exec($query);
        $messages[] = replace_escaped_vars($language->install_messages->table_created_ok, '{$table_name}', 'categories');
    }
    catch( \Exception $e )
    {
        $errors[] = replace_escaped_vars(
            $language->install_messages->table_created_ko,
            array('{$table_name}', '{$error}'),
            array('categories', $e->getMessage())
        );
    }
    
    $database->exec("
        insert ignore into categories set
          id_category     = '0000000000000',
          parent_category = '',
          slug            = 'default',
          title           = 'Default category',
          description     = 'Uncategorized items',
          visibility      = 'public',
          min_level       = '0'
    ");
    
    return;
}

if( $module_install_action == "uninstall" )
{
    $query = "DROP TABLE IF EXISTS categories";
    
    try
    {
        $database->exec($query);
        $messages[] = replace_escaped_vars($language->install_messages->table_deleted_ok, '{$table_name}', 'posts');
    }
    catch( \Exception $e )
    {
        $errors[] = replace_escaped_vars(
            $language->install_messages->table_deleted_ko,
            array( '{$table_name}', '{$error}'         ),
            array( 'categories',      $e->getMessage() )
        );
    }
    
    return;
}
