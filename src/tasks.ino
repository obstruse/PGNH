
void tasks_backgroundProcessesTask( void *pvParameters )
{
  for ( ;; )
  {
    // Does some background tasks - reading touches, redrawing screen, 
    // checking for idle.
    // xSemaphoreTake(xMutex, portMAX_DELAY);
    impl_runBackgroundProcesses();
    // xSemaphoreGive(xMutex);   
    taskYIELD();

  }
  vTaskDelete( NULL );
}


void tasks_startTasks() {
  
  xTaskCreate(tasks_backgroundProcessesTask,"tasks_backgroundProcessesTask",8000,NULL,1,&backgroundProcessesTaskHandle);

}