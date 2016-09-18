<?php
namespace hng2_modules\categories;

use hng2_repository\abstract_record;

class category_record extends abstract_record
{
    public $id_category;
    
    public $parent_category;
    
    public $slug;
    
    public $title;
    
    public $description;
    
    /**
     * @var string public|users|level_based
     */
    
    public $visibility;
    
    public $min_level;
    
    public $parent_category_slug;
    public $parent_category_title;
    
    public function set_new_id()
    {
        list($sec, $usec) = explode(".", microtime(true));
        $this->id_category = "1020" . $sec . sprintf("%05.0f", $usec) . mt_rand(1000, 9999);;
    }
}
