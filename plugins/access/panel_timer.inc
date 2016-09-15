<?php
/**
 * @file
 * Plugin the view content based on time
 */

$plugin = array(
  'title'            => t('Timer'),
  'description'      => t('Check timer for viewing data.'),
  'callback'         => 'panel_timer_ctools_access_check',
  'summary'          => 'panel_timer_ctools_access_summary',
  'settings form'    => 'panel_timer_ctools_access_settings',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);


/**
 * Settings form for the 'Panel Timer' access plugin.
 */
function panel_timer_ctools_access_settings(&$form, &$form_state) {

  $panel_id = arg(5);

  $timer_query = db_select('panel_timer', 't')
                   ->fields('t')
                   ->condition('panel_id', $panel_id, '=')
                   ->execute()
                   ->fetchObject();
  //dpm($timer_query);

  $days = array(
    0 => 'Sunday',
    1 => 'Monday',
    2 => 'Tuesday',
    3 => 'Wednesday',
    4 => 'Thrusday',
    5 => 'Friday',
    6 => 'Saturday'
  );
  $form['settings']['startTime'] = array(
    '#title'         => t('Start Time'),
    '#type'          => 'textfield',
    '#description'   => t('Start Time.'),
    '#default_value' => $timer_query->start_time,
    '#required'      => TRUE,
  );
  $form['settings']['endTime'] = array(
    '#title'         => t('End Time'),
    '#type'          => 'textfield',
    '#description'   => t('End Time.'),
    '#default_value' => $timer_query->end_time,
    '#required'      => TRUE,
  );
  $days_set = json_decode($timer_query->days);

  $form['settings']['days'] = array(
    '#title'         => t('Days'),
    '#type'          => 'checkboxes',
    '#options'       => $days,
    '#description'   => t('Days Activated.'),
    '#default_value' => $days_set,
    '#required'      => TRUE,
  );

  $form['settings']['panel_id'] = array(
    '#type'          => 'hidden',
    '#default_value' => arg(5),
    '#required'      => TRUE,
  );

  $form['#submit'][] = panel_timer_submit;
  return $form;
}

/**
 * Form Submit
 */
function panel_timer_submit($form, $form_state, $conf) {
  $days = array();
  foreach ($form_state['input']['settings']['days'] as $key => $value) {
    if ($value <> NULL) {
       $day[] = $value;
    }
  }

  $days = json_encode($day);
  db_delete('panel_timer')
   ->condition('panel_id', $form_state['input']['settings']['panel_id'])
   ->execute();

  db_insert('panel_timer')
        ->fields(array(
         'panel_id' => $form_state['input']['settings']['panel_id'],
         'start_time' => $form_state['input']['settings']['startTime'],
         'end_time' => $form_state['input']['settings']['endTime'],
         'days' => $days,
          ))
        ->execute();
}

/**
 * Provide a summary description.
 */
function panel_timer_ctools_access_summary($conf, $context) {
  $time = array('@timer' => 1);
    return t('Checks the time.', $time);
}

/**
 * Render callback function.
 *
 * @param $subtype
 * @param $conf
 * @param $args
 * @param $context
 * @return stdClass
 */
function panel_timer_ctools_access_check($subtype, $conf, $args, $context) {
  dpm($subtype);
  dpm($args);
  dpm($conf);
  dpm($context);
     $query = db_query('SELECT panel_id, start_time, end_time, days FROM {panel_timer}')->fetchAll();
     dpm($query);
     foreach ($query as $key => $row) {
       dpm('panel_id: ' . $row->panel_id);
     }

      return TRUE;

}
