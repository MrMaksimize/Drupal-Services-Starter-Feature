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
  $endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'bencode' => TRUE,
        'json' => TRUE,
        'jsonp' => TRUE,
        'php' => TRUE,
        'rss' => TRUE,
        'xml' => TRUE,
        'yaml' => TRUE,
      ),
      'parsers' => array(
        'application/json' => TRUE,
        'application/vnd.php.serialized' => TRUE,
        'application/x-www-form-urlencoded' => TRUE,
        'application/x-yaml' => TRUE,
        'multipart/form-data' => TRUE,
      ),
    ),
  );
  $endpoint->resources = array(
    'node' => array(
      'alias' => 'node',
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'relationships' => array(
        'files' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'system' => array(
      'alias' => 'system',
      'actions' => array(
        'connect' => array(
          'enabled' => 1,
        ),
        'get_variable' => array(
          'enabled' => 1,
        ),
        'set_variable' => array(
          'enabled' => 1,
        ),
        'del_variable' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'user' => array(
      'alias' => 'user',
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'actions' => array(
        'login' => array(
          'enabled' => 1,
        ),
        'logout' => array(
          'enabled' => 1,
        ),
        'register' => array(
          'enabled' => 1,
        ),
      ),
    ),
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
