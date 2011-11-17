<?php
/**
 * @file
 * Loads dependencies and defines the apci_events resource
 */
module_load_include('inc', 'services_views', 'services_views.resource');
module_load_include('inc', 'services', 'resources/node_resource');
function services_demo_events_resource_definition() {
  $events_resource = array(
    'events' => array(
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'services_demo', 'name' => 'resources/events_resource'),
        'callback' => '_services_demo_events_retrieve',
        'help' => 'Retrieve Event by eid',
        'args' => array(
          array(
            'name' => 'eid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The event eid you would like to retrieve',
          ),
          array(
            'name' => 'fields',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The fields to get.',
            'default value' => '*',
            'source' => array('param' => 'fields'),
          ),
        ),
        'access callback' => '_services_demo_events_access',
        'access arguments' => array('retrieve'),
        'access arguments append' => TRUE,
      ), // retrieve
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'services_demo', 'name' => 'resources/events_resource'),
        'callback' => '_services_demo_events_index',
        'help' => 'retrieve a set of events',
        'args' => array(
          array(
            'name' => 'date',
            'optional' => TRUE,
            'source' => array('param' => 'date'),
            'type' => 'string',
            'description' => 'Pass in format 2011-11',
          ),
          array(
            'name' => 'fields',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The fields to get.',
            'default value' => '*',
            'source' => array('param' => 'fields'),
          ),
        ),
        'access callback' => '_services_demo_events_access',
        'access arguments' => array('index'),
        'access arguments append' => TRUE,
      ),
      'create' => array(
        'help' => 'Create a new event',
        'callback' => '_services_demo_events_create',
        'file' => array('type' => 'inc', 'module' => 'services_demo', 'name' => 'resources/events_resource'),
        'args' => array(
          array(
            'name' => 'participants',
            'type' => 'array',
            'description' => 'UIDs of Participants',
            'source' => 'data',
            'optional' => TRUE,
          ), // gid
          array(
            'name' => 'title',
            'type' => 'string',
            'description' => 'Event Title',
            'source' => 'data',
            'optional' => FALSE,
          ), // subject
          array(
            'name' => 'description',
            'type' => 'string',
            'description' => 'Event description',
            'source' => 'data',
            'optional' => FALSE,
          ), // body
          array(
            'name' => 'date_time',
            'type' => 'array',
            'description' => 'an array containing data for the date',
            'source' => 'data',
            'optional' => FALSE,
          ), // date_end
        ), // args
        'access callback' => '_services_demo_events_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
      ), // create
      'update' => array(
        'help' => 'Update an event thats been created',
        'callback' => '_services_demo_events_update',
        'file' => array('type' => 'inc', 'module' => 'services_demo', 'name' => 'resources/events_resource'),
        'args' => array(
          array(
            'name' => 'participants',
            'type' => 'array',
            'description' => 'UIDs of Participants',
            'source' => 'data',
            'optional' => TRUE,
          ), // gid
          array(
            'name' => 'title',
            'type' => 'string',
            'description' => 'Event Title',
            'source' => 'data',
            'optional' => TRUE,
          ), // subject
          array(
            'name' => 'description',
            'type' => 'string',
            'description' => 'Event description',
            'source' => 'data',
            'optional' => TRUE,
          ), // body
          array(
            'name' => 'date_time',
            'type' => 'array',
            'description' => 'an array containing data for the date',
            'source' => 'data',
            'optional' => TRUE,
          ), // date_end
        ), // args
        'access callback' => '_services_demo_events_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
      ), // update
    ), // apci_events
  ); // end events_resource
  return $events_resource;
}

/**
 * Events Access Callback
 */

function _services_demo_events_access($op, $args){
  return TRUE;
}

/**
 * Events Resource Callbacks
 */
function _services_demo_events_retrieve($eid, $fields) {
  global $user;
  $event = node_load($eid);
  if (!$event){
    return services_error(t('incorrect event id'), 404);
  }
  return _services_demo_events_return_process($event, 'retrieve');
}

function _services_demo_events_index($date, $fields){
  global $user;
  $events_list = services_views_retrieve($view_name = 'services_demo_events', $display_id = 'events_index', $args = array($date), $offset = 0, $limit = 10, $return_type = FALSE);
  return _services_demo_events_return_process($events_list, 'index');
  //return $events_list;
}

/**
 * Events Process
 */
function _services_demo_events_return_process($data, $op){
  switch ($op){
    case 'retrieve':
      /*preprocess fields*/
      $description = current($data->body['und']);
      $dates = current($data->field_date['und']);
      $return = array(
        'title' => $data->title,
        'description' => $description['value'],
        'start' => $dates['value'],
        'end' =>  $dates['value2'],
        'timezone' => $dates['timezone'],
        'attendees' => $data->field_attendees,
      );
    break;
    case 'index':
      foreach ($data as $event){
        $body = current($event->field_body);
        $body = $body['raw']['value'];
        $dates = current($event->field_field_date);
        $dates = $dates['raw'];

        $ev_ret = array(
          'eid' => $event->nid,
          'title' => $event->node_title,
          'body' => $body,
          'start' => $dates['value'],
          'end' => $dates['value2'],
          'timezone' => $dates['timezone'],
          'uri' => services_resource_uri(array('events', $event->nid)),
        );
        $return[] = $ev_ret;
      }
    break;
  }
  return $return;
}

