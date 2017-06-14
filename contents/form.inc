<?php
/**
 * Categories editing form
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *             
 * @var module $current_module
 */

use hng2_base\module;

$current_module->load_extensions("form", "before_rendering");
?>

<div id="category_form_target" style="display: none;"></div>

<div class="multicol cols-2">
    <form name="category_form" id="category_form" method="post"
          action="<?= $config->full_root_path ?>/categories/scripts/save.php">
        
        <input type="hidden" name="id_category" value="">
        
        <div class="col">
            <? $info_handler = "{$current_module->name}_info_title"; ?>
            <div class="field" data-field="title">
                <div class="caption">
                    <?= $current_module->language->form->fields->title->caption ?>
                    <span class="pseudo_link" onclick="toggle_info_section('<?= $info_handler ?>')"><span
                          class="fa fa-info-circle" title="<?= $language->toggle_info ?>"></span></span>
                </div>
                <div class="input">
                    <input type="text" name="title" value="" maxlength="255">
                </div>
                <div class="input info_handler" id="<?= $info_handler ?>"
                     style="<? if($account->engine_prefs[$info_handler] == "hidden") echo "display: none;"; ?>">
                    <span class="fa fa-info-circle"></span>
                    <?= $current_module->language->form->fields->title->info ?>
                </div>
            </div>
            
            <? $info_handler = "{$current_module->name}_info_slug"; ?>
            <div class="field" data-field="slug">
                <div class="caption">
                    <?= $current_module->language->form->fields->slug->caption ?>
                    <span class="pseudo_link" onclick="toggle_info_section('<?= $info_handler ?>')"><span
                          class="fa fa-info-circle" title="<?= $language->toggle_info ?>"></span></span>
                </div>
                <div class="input">
                    <input type="text" name="slug" value="" maxlength="128">
                </div>
                <div class="input info_handler" id="<?= $info_handler ?>"
                     style="<? if($account->engine_prefs[$info_handler] == "hidden") echo "display: none;"; ?>">
                    <span class="fa fa-info-circle"></span>
                    <?= $current_module->language->form->fields->slug->info ?>
                </div>
            </div>
            
            <? $info_handler = "{$current_module->name}_info_description"; ?>
            <div class="field" data-field="description">
                <div class="caption">
                    <?= $current_module->language->form->fields->description->caption ?>
                    <span class="pseudo_link" onclick="toggle_info_section('<?= $info_handler ?>')"><span
                          class="fa fa-info-circle" title="<?= $language->toggle_info ?>"></span></span>
                </div>
                <div class="input">
                    <textarea name="description" class="expandible_textarea" style="min-height: 38px;"></textarea>
                </div>
                <div class="input info_handler" id="<?= $info_handler ?>"
                     style="<? if($account->engine_prefs[$info_handler] == "hidden") echo "display: none;"; ?>">
                    <span class="fa fa-info-circle"></span>
                    <?= $current_module->language->form->fields->description->info ?>
                </div>
            </div>
            
            <? $current_module->load_extensions("form", "leftcol_additions"); ?>
        </div>
        
        <div class="col">
            <? $info_handler = "{$current_module->name}_info_visibility"; ?>
            <div class="field" data-field="visibility">
                <div class="caption">
                    <?= $current_module->language->form->fields->visibility->caption ?>
                    <span class="pseudo_link" onclick="toggle_info_section('<?= $info_handler ?>', true)"><span
                          class="fa fa-info-circle" title="<?= $language->toggle_info ?>"></span></span>
                </div>
                <div class="input">
                    
                    <? $option = $current_module->language->form->fields->visibility->options->public ?>
                    <label>
                        <input type="radio" name="visibility" value="public" onclick="$('#visibility_min_level').hide()">
                        <?= $option->caption ?>
                    </label><br>
                    <div class="input info_handler" id="<?= "{$info_handler}_public" ?>"
                         style="<? if($account->engine_prefs[$info_handler] == "hidden") echo "display: none;"; ?>">
                        <?= $option->info ?>
                    </div>
                    
                    <? $option = $current_module->language->form->fields->visibility->options->users ?>
                    <label>
                        <input type="radio" name="visibility" value="users" onclick="$('#visibility_min_level').hide()">
                        <?= $option->caption ?>
                    </label><br>
                    <div class="input info_handler" id="<?= "{$info_handler}_users" ?>"
                         style="<? if($account->engine_prefs[$info_handler] == "hidden") echo "display: none;"; ?>">
                        <?= $option->info ?>
                    </div>
                    
                    <? $option = $current_module->language->form->fields->visibility->options->level_based ?>
                    <label>
                        <input type="radio" name="visibility" value="level_based" onclick="$('#visibility_min_level').show()">
                        <?= $option->caption ?>
                    </label><br>
                    <div class="input info_handler" id="<?= "{$info_handler}_level_based" ?>"
                         style="<? if($account->engine_prefs[$info_handler] == "hidden") echo "display: none;"; ?>">
                        <?= $option->info ?>
                    </div>
                    <div class="input buttonized_radios" id="visibility_min_level" style="display: none;">
                        <? foreach($config->user_levels_by_level as $level => $name):
                            if( $level <= 1 ) continue; ?>
                            <label class="framed_content inlined">
                                <input type="radio" name="min_level" value="<?= $level ?>">
                                <?= $name ?> (<?= $level ?>)
                            </label>
                        <? endforeach ?>
                    </div>
                </div>
            </div>
            
            <? $info_handler = "{$current_module->name}_info_parent_category"; ?>
            <div class="field" data-field="description">
                <div class="caption">
                    <?= $current_module->language->form->fields->parent_category->caption ?>
                    <span class="pseudo_link" onclick="toggle_info_section('<?= $info_handler ?>')"><span
                          class="fa fa-info-circle" title="<?= $language->toggle_info ?>"></span></span>
                </div>
                <div class="input" id="parent_category_selector_container">
                    <select name="parent_category" class="big_monospace_select">
                        <option value=""><?= $current_module->language->form->fields->parent_category->none ?></option>
                    </select>
                </div>
                <div class="input info_handler" id="<?= $info_handler ?>"
                     style="<? if($account->engine_prefs[$info_handler] == "hidden") echo "display: none;"; ?>">
                    <span class="fa fa-info-circle"></span>
                    <?= $current_module->language->form->fields->parent_category->info ?>
                </div>
            </div>
            
            <? $current_module->load_extensions("form", "rightcol_additions"); ?>
        </div>
        
        <? $current_module->load_extensions("form", "before_submit_button"); ?>
        
        <p align="center">
            <button type="submit" class="big_form_submit_button">
                <span class="fa fa-save"></span>
                <?= $language->save ?>
            </button>
            <button type="reset"  class="big_form_submit_button" onclick="hide_category_form()">
                <span class="fa fa-ban"></span>
                <?= $language->cancel ?>
            </button>
        </p>
        
    </form>
</div>
