
function reset_filter()
{
    var $form = $('#filter_form');
    $form.find('input[name="search_for"]').val('');
    $form.find('select[name="limit"] options:first').prop('selected', true);
    $form.find('input[name="offset"]').val('0');
    $form.find('input[name="order"]').val('3');
}

//noinspection JSUnusedGlobalSymbols
function paginate(value)
{
    var $form = $('#filter_form');
    $form.find('input[name="offset"]').val(value);
    $form.submit();
}

function prepare_category_addition()
{
    $('#form_workarea')
        .find('.for_edition').hide()
        .find('.for_addition').show()
    ;
    reset_category_form();
    show_category_form();
    update_category_selector();
}

function prepare_category_edition(id_category)
{
    
    
    
    
    
    
    reset_category_form();
    show_category_form();
    update_category_selector();
}

function update_category_selector()
{
    var $container = $('#parent_category_selector_container');
    $container.block(blockUI_smallest_params);
    
    var url = $_ROOT_URL + '/categories/scripts/tree_as_json.php?wasuuup=' + parseInt(Math.random() * 1000000000000000);
    $.getJSON(url, function(data)
    {
        if( data.message != 'OK' )
        {
            alert(data.message);
            $container.unblock();
            
            return;
        }
        
        var $select = $container.find('select');
        $select.find('option:not(:first)').remove();
        
        for(var key in data.data)
            $select.append('<option value="' + key + '">' + data.data[key] + '</option>');
        
        $container.unblock();
    });
}

function reset_category_form()
{
    // Todo: implement reset_category_addition_form() function
    var $form = $('#category_form');
    $form[0].reset();
    $form.find('input[name="slug"]').data('modified', false);
}

function show_category_form()
{
    $('#main_workarea').hide('fast');
    $('#form_workarea').show('fast');
}

function hide_category_form()
{
    $('#form_workarea').hide('fast');
    $('#main_workarea').show('fast');
}

function update_slug()
{
    var $form = $('#category_form');
    if( $form.find('input[name="id_category"]').val() != '' ) return;
    if( $form.find('input[name="slug"]').data('modified') ) return;
    
    var title = $form.find('input[name="title"]');
    var slug  = title.toLowerCase();
    
    slug = slug.replace(/[^a-z0-9\-_]/g, "-");
    slug = slug.replace(/\-+/g, "-");
    slug = slug.replace(/_+/g, "_");
    
    $form.find('input[name="slug"]').val(slug);
}

function prepare_category_form_submission()
{
    $.blockUI(blockUI_default_params);
}

function process_category_form_response(response)
{
    $.unblockUI();
    if( response != 'OK' )
    {
        alert( response );
        return;
    }
    
    hide_category_form();
    $('#refresh_category_browser').click();
}

$(document).ready(function()
{
    $('#category_form').ajaxForm({
        target: '#category_form_target',
        beforeSubmit: prepare_category_form_submission,
        success:      process_category_form_response
    });
});
