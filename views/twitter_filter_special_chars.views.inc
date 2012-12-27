<?php

/**
 * @file
 * Provide views data for twitter_filter_special_chars.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data_alter().
 */
function twitter_filter_special_chars_views_data_alter(&$data) {

  $data['twitter']['text']['field']['handler'] = 'twitter_views_handler_field_xss_sc';

}
