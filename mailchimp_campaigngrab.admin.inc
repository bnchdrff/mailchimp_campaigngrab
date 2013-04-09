<?php

/**
 * @file mailchimp_campaigngrab.admin.inc
 */


/**
 * Implements hook_admin().
 */
function mailchimp_campaigngrab_admin() {
  $webhook_instructions = '<p>' . t('By supplying the following URL in a MailChimp List Webooks setting, campaigns will be automatically created in Drupal when they are sent:') . '</p>';
  $webhook_instructions .= '<input type="text" readonly="readonly" size="120" value="' . mailchimp_webhook_url() . '" />';
  $webhook_instructions .= '<p>' . t('Be sure to check the "Campaign Sending" option in the MailChimp Webhooks "What Types Of Updates Should We Send?" section.') . '</p>';

  $form = array();

  $form['mailchimp_campaigngrab_templateregions'] = array(
    '#type' => 'textfield',
    '#title' => t('Template regions to include in node creation'),
    '#default_value' => variable_get('mailchimp_campaigngrab_templateregions', 'main'),
    '#size' => 40,
    '#maxlength' => 64,
    '#description' => t("The MailChimp template region names to copy into a new node using the 'Create Node' action, comma separated."),
    '#required' => FALSE,
    '#multiple' => FALSE,
    '#prefix' => $webhook_instructions,
    '#element_validate' => array('_validate_templateregions'),
  );

  $form['mailchimp_campaigngrab_nodetype'] = array(
    '#type' => 'select',
    '#title' => t('Node type to create'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('mailchimp_campaigngrab_nodetype', 'weekly_update'),
    '#description' => t('Node type to create when using the "Create Node" action.'),
    '#element_validate' => array('_validate_nodetype'),
  );

  return system_settings_form($form);

}

/**
 * Helper validation function for node type config option
 */
function _validate_nodetype($element, &$form_state) {
  if (!empty($element['#value']) && !array_key_exists($element['#value'], node_type_get_names())) {
    form_error($element, t('Please provide a valid content type'));
  }
}

/**
 * Helper validation function for template regions config option
 */
function _validate_templateregions($element, &$form_state) {
  if (!empty($element['#value']) && preg_match('/a-zA-Z_,/', $element['#value'])) {
    form_error($element, t('Please provide a comma-separated list of template regions'));
  }
}

/**
 * Copy campaign to a new node.
 *
 * @param object $campaign
 *   MailChimpCampaign object
 */
function mailchimp_campaigngrab_createnode($campaign) {
  $regions = _mailchimp_campaigngrab_templateregions($campaign->template);
  $body = '';

  foreach ($regions as $region) {
    $body .= $region['value'];
  }

  $node = new StdClass();
  $node->type = variable_get('mailchimp_campaigngrab_nodetype', 'weekly_update');
  $node->title = 'Newsletter: ' . date('F j, Y', $campaign->created);
  $node->language = LANGUAGE_NONE;
  $node->status = 0;
  $node->body[$node->language][0]['value']   = $body;
  $node->body[$node->language][0]['summary'] = text_summary($body);
  $node->body[$node->language][0]['format']  = 'mailchimp_campaign';
  $node->created = $campaign->created;

  node_save($node);

  drupal_goto('node/' . $node->nid . '/edit');
}


