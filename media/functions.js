
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
    show_category_addition_form();
}

function show_category_addition_form()
{
    $('#main_workarea').hide('fast');
    $('#form_workarea').show('fast');
}

function hide_category_addition_form()
{
    $('#form_workarea').hide('fast');
    $('#main_workarea').show('fast');
}
