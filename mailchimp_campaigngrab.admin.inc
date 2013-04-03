<?php

function mailchimp_campaigngrab_webhookurl_page() {
  $output = '<p>' . t('By supplying the following URL in a MailChimp List Webooks setting, campaigns will be automatically created in Drupal when they are sent:') . '</p>';
  $output .= '<input type="text" readonly="readonly" size="120" value="' . mailchimp_webhook_url() . '" />';
  $output .= '<p>' . t('Be sure to check the "Campaign Sending" option in the MailChimp Webhooks "What Types Of Updates Should We Send?" section.') . '</p>';
  return array('#markup' => $output);
}

