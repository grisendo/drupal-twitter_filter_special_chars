<?php
/**
 * @file
 * Views handlers for Twitter module.
 */

/**
 * Process Twitter-style @usernames and URLs before filtering XSS.
 */
class twitter_views_handler_field_xss_sc extends views_handler_field {

  /**
   * Information about options for all kinds of purposes will be held here.
   * @code
   * 'option_name' => array(
   *  - 'default' => default value,
   *  - 'translatable' => (optional) TRUE/FALSE (wrap in t() on export if true),
   *  - 'contains' => (optional) array of items this contains, with its own
   *      defaults, etc. If contains is set, the default will be ignored and
   *      assumed to be array().
   *  - 'bool' => (optional) TRUE/FALSE Is the value a boolean value. This will
   *      change the export format to TRUE/FALSE instead of 1/0.
   *  - 'export' => (optional) FALSE or a callback for special export handling
   *      if necessary.
   *  - 'unpack_translatable' => (optional) callback for special handling for
   *      translating data within the option, if necessary.
   *  ),
   *
   * @return array
   *   Returns the options of this handler/plugin.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['link_urls'] = array('default' => TRUE);
    $options['link_usernames'] = array('default' => TRUE);
    $options['link_hashtags'] = array('default' => FALSE);
    $options['hashtags_url'] = array('default' => variable_get('twitter_search', TWITTER_SEARCH) . '/search?q=%23');
    return $options;
  }

  /**
   * Build the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_urls'] = array(
      '#title' => t('Link urls to their destinations'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_urls']),
    );
    $form['link_usernames'] = array(
      '#title' => t('Link Twitter @usernames to their Twitter.com urls'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_usernames']),
    );
    $form['link_hashtags'] = array(
      '#title' => t('Link Twitter #hashtags to another url'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_hashtags']),
    );
    $form['hashtags_url'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->options['hashtags_url'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-options-link-hashtags' => array(TRUE)),
    );
  }

  /**
   * Render the field.
   *
   * @param stdClass $values
   *   The values retrieved from the database.
   */
  public function render($values) {
    $value = $values->{$this->field_alias};
    if (!empty($this->options['link_urls'])) {
      $filter = new stdClass;
      $filter->settings = array(
        'filter_url_length' => 496,
      );
     $value = _filter_url($value, $filter);
    }
    if (!empty($this->options['link_usernames'])) {
      $value = _twitter_filter_special_chars_filter_text($value, '@', variable_get('twitter_host', TWITTER_HOST) . '/');
    } if (!empty($this->options['link_hashtags']) && valid_url($this->options['hashtags_url'])) {
      $value = _twitter_filter_special_chars_filter_text($value, '#', url($this->options['hashtags_url']));
    }
    return filter_xss($value);
  }

}

if (!class_exists('twitter_views_handler_field_profile_image')) {

  /**
   * Field handler to provide simple renderer that turns a URL into a clickable link.
   */
  class twitter_views_handler_field_profile_image extends views_handler_field {

    public function render($values) {
      $value = $values->{$this->field_alias};
      return theme('image', array('path' => $value));
    }

  }

}
