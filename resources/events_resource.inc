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
/**
 * Returns an array of groups.
 * @param type $gids
 * @return type
 */
function _apci_api_events_get_groups($gids) {
  $groups = array();

  // Process groups
  if ($gids != NULL) {

    // Get the Groups from the UUID string.
    $groups = apci_public_api_services_get_entities($gids, 'group', function($group) {
      if (!og_is_group_member($group->nid)) {
        services_error(t("You must be a member of @gid to update events here", array('@gid' => $group->nid)), 400);
        return false;
      }
      return true;
    });

    $gids = array_keys($groups);
    $groups = array_combine($gids, $gids);
  }

  return $groups;
}



/**
 * Updates an event node
 */
function _apci_api_events_update($uuid, $gids, $title, $description, $date_time, $category, $resources, $competitors) {
  module_load_include('inc', 'apci_public_api_services', 'date_handling');

  // Load the event object.
  $event = (array)node_get_by_uuid($uuid);

  // Check to make sure this is an event
  if ($event['type'] != 'vevent') {
    return services_error(t('This is not an event node'));
  }

  $event = _apci_get_event_event($event, $gids, $title, $description, $date_time, $category, $resources, $competitors);
  module_load_include('inc', 'services', 'resources/node_resource');
  $resource = _node_resource_update($event['nid'], $event);

  // Get the UUID.
  $uuid = db_result(db_query("SELECT uuid FROM {uuid_node} WHERE nid=%d", $resource['nid']));

  // Return the created event.
  return _apci_api_events_retrieve($uuid, array(), TRUE);
}
/**
 * Creates an event
 *
 * @param string $gids
 *   A string containing a comma separated values of group UUIDs in which to create the event
 *
 * @param string $title
 *   A string containing the new title for event.  This gets overriden if the category is
 *   game or scrimmage
 *
 * @param string $description
 *   A string containing the description of the event
 *
 * @param array $date_time
 *   an array containing date start, date end, and repetition parameters
 *  @see date_handling.inc _apci_api_date_build
 * @param string $category
 *   A string that determines what category the event is placed in.
 *   Options are: Game, Meeting, Other, Party, Practice, Scrimmage
 *   Game and Scrimmage categories require $competitors array to be passed and
 *   will override the title
 *
 * @param string $resources
 *   A string of comma separated resource UUIDs
 *
 * @param array $competitors
 *   an array containing labels and scores keyed by event;
 *   Example array:
 *   @codeY.
 *    $competitors[103118] = array(
 *      'label' => something
 *      'score' => 33
 *    )
 *   @endcode
 */

function _apci_api_events_create($gids, $title, $description, $date_time, $category, $resources, $competitors) {
  global $user;

  // create an event array
  $event = array(
    'type' => 'vevent',
    'sticky' => 0,
    'status' => 1,
    'promote' => 1,
    'language' => 'en', // @todo get from settings
    'uid' => $user->uid,
    'name' => $user->name,
    'og_public' => TRUE,
    'notifications_content_disable' => TRUE
  );

  $event = _apci_get_event_event($event, $gids, $title, $description, $date_time, $category, $resources, $competitors);
  module_load_include('inc', 'services', 'resources/node_resource');
  $resource = _node_resource_create($event);

  // Get the UUID.
  $uuid = db_result(db_query("SELECT uuid FROM {uuid_node} WHERE nid=%d", $resource['nid']));

  // Return the created event.
  return _apci_api_events_retrieve($uuid, array(), TRUE);
}

/**
 * Events Resource Preparation Functions
 */
function _apci_api_events_prepare($event, $filters = NULL) {
  date_default_timezone_set('UTC');
  module_load_include('inc', 'apci_public_api_services', 'resources/resources_resource');
  $event = _apci_api_events_perms_check($event);
  $return = array(
    'uuid' => $event->uuid,
    'description' => $event->body,
    'title' => $event->title,
    'groups' => $event -> og_groups,
    // 'date_time'=> $event->field_date,
    // 'test'=> strtotime($event->field_date['0']['value']),
  );
  // process dates array
  /*dates return fine at the theme layer, but in the returns they don't.
   so i'm making them */
  $date_time = array();
  foreach ($event->field_date as $date) {
    $date['value'] = date_timezone_set(date_create($date['value']), timezone_open($date['timezone']));
    $date['value2'] = date_timezone_set(date_create($date['value2']), timezone_open($date['timezone']));
    $dt = array(
      'start' => date_format_date($date['value'], 'custom', 'Y-m-d\TG:i:s P'),
      'start_timestamp' => date_timestamp_get($date['value']),
      'end' => date_format_date($date['value2'], 'custom', 'Y-m-d\TG:i:s P'), // @todo timezone here
      'end_timestamp' => date_timestamp_get($date['value2']),
    );
    $date_time[] = $dt;
  }
  $return['date_time'] = $date_time;
  $competitors = array(
    'groups' => $event->field_sports_competitor_group,
    'labels' => $event->field_sports_competitor_label,
    'results' => $event->field_sports_competitor_result,
  );

  $return['category'] = _apci_api_events_process_category($event->taxonomy, 'from_node');
  $return['competitors'] = _apci_api_events_process_competitors($competitors, 'from_node');
  // process resources
  $resource_ids = array();
  if ($event->field_ref_resource['0']['nid'] != NULL){
    foreach ($event->field_ref_resource as $rid) {
      $res = node_load($rid['nid']);  // @todo here (?)
      $res = _apci_api_resources_retrieve($res->uuid, 'uuid,title,location');
      $resource_ids[$rid['uuid']] = $res;
    }
  }
  $return['resource_ids'] = $resource_ids;
  if ($filters && $filters[0] != '*') {
    $filters = explode(',', $filters);
    $filters = array_flip($filters);
    $return = array_intersect_key($return, $filters);
  }
  return $return;
}

function _apci_api_events_process_category($category, $call) {
  // process category and create into a taxonomy array
  // get term by name
  // if the call is coming into the node, it will come as a string.
  if ($call == 'to_node') {
    $taxonomy = array('38' => '');
    if (!$category || $category == NULL){
      return $taxonomy;
    }
    $category = ucfirst(strtolower($category));
    $tax_search = taxonomy_get_term_by_name($category);
    // search through results
    foreach ($tax_search as $match) {
      if ($match->vid == '38') {
        $taxonomy['38'] = $match->tid;
        break;
      }
    }
  }
  // if the data is coming from the node, it will come as an array, and we get the string
  elseif ($call == 'from_node') {
    $taxonomy = current($category);
    $taxonomy = strtolower($term->name);
  }
  return $taxonomy;
}

function _apci_api_events_process_competitors($competitors, $call){
  if ($call == 'to_node') {
    $competitors_labels = array();
    $competitors_groups = array();
    $competitors_scores = array();
    $count = 0;
    $title = NULL; // reset title
    foreach ($competitors as $gid => $data) {
      $competitors_labels[$count]['value'] = $data['label'];
      $competitors_groups[$count]['nid'] = $gid;
      if (isset($data['score']) && is_numeric($data['score'])) {
        $competitors_scores[$count]['score'] = $data['score'];
      }
      else{
        $competitors_scores[$count]['score'] = '';
      }
      $count++;
    }
    $return['competitors_labels'] = $competitors_labels;
    $return['competitors_groups'] = $competitors_groups;
    $return['competitors_scores'] = $competitors_scores;
  }
  elseif ($call == 'from_node') {
    $competitor_data = array();
    foreach ($competitors['groups'] as $key => $competitor) {
      $competitor_data[$competitor['nid']] = array(
        'label' => $competitors['labels'][$key]['value'],
        'score' => $competitors['results'][$key]['score'],
        'event_outcome' => $competitors['results'][$key]['event_outcome'],
      );
    }
    $return = $competitor_data;  // @todo fix!!!
  }
  return $return;
}