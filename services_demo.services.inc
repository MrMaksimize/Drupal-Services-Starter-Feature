<?php
/**
 * @file
 * services_demo.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function services_demo_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass;
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'services_demo_endpoint';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'api';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = '';
  $endpoint->resources = array(
    'events' => array(
      'alias' => 'events',
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 1;
  $export['services_demo_endpoint'] = $endpoint;

  return $export;
}
