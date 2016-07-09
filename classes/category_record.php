<?php
namespace hng2_modules\categories;

use hng2_base\repository\abstract_record;

class category_record extends abstract_record
{
    public $id_category;
    public $parent_category;
    public $slug;
    public $title;
    public $description;
    public $visibility;
    public $min_level;
    
    public $parent_category_slug;
    public $parent_category_title;
    
    public function set_new_id()
    {
        $this->id_category = uniqid();
    }
}
