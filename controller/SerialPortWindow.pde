/*------------------------------------------------------------------------
 Class and controllers on the "serial port" subwindow
 ------------------------------------------------------------------------*/
  
ControlFrameSimple addSerialPortControlFrame(String theName, int theWidth, int theHeight, int theX, int theY, int theColor ) {
  final Frame f = new Frame( theName );
  final ControlFrameSimple p = new ControlFrameSimple( this, theWidth, theHeight, theColor );

  f.add( p );
  p.init();
  f.setTitle(theName);
  f.setSize( p.w, p.h );
  f.setLocation( theX, theY );
  f.addWindowListener( new WindowAdapter() {
    @Override
      public void windowClosing(WindowEvent we) {
      p.dispose();
      f.dispose();
    }
  } 
  );
  f.setResizable( true );
  f.setVisible( true );
  // sleep a little bit to allow p to call setup.
  // otherwise a nullpointerexception might be caused.
  try {
    Thread.sleep( 100 );
  } 
  catch(Exception e) {
  }

//  ScrollableList sl = p.cp5().addScrollableList("dropdown_serialPort")
  Textfield sl = p.cp5().addTextfield("Network Host")
    .setPosition(10, 10)
    .setSize(150, 20)
//    .setBarHeight(20)
//    .setItemHeight(16)
    .setAutoClear(false)
    .plugTo(this, "dropdown_serialPort");  

//  sl.addItem("No serial connection", -1);

//  String[] ports = Serial.list();
  
//  for (int i = 0; i < ports.length; i++) {
//    println("Adding " + ports[i]);
//    sl.addItem(ports[i], i);
//  }
  
//  int portNo = getSerialPortNumber();
//  println("portNo: " + portNo);
//  if (portNo < 0 || portNo >= ports.length)
//    portNo = -1;

  // set the value of the actual control
  // sl.setValue(portNo);
  sl.setValue(getNetworkHost() );

//  sl.setOpen(false);
  return p;
}


//void dropdown_serialPort(int newSerialPort) 
void dropdown_serialPort(String newNetworkHost) 
{
  
  //println("In dropdown_serialPort, newSerialPort: " + newSerialPort);
  println("In dropdown_serialPort, newNetworkHost: " + newNetworkHost);
/*
  // No serial in list is slot 0 in code because of list index
  // So shift port index by one 
  newSerialPort -= 1;
  
  if (newSerialPort == -2)
  {
  } 
  else if (newSerialPort == -1) {
*/

  println("Disconnecting network host.");
  useSerialPortConnection = false;
  if (myPort != null) {
    myPort.stop();
    myPort = null;
  }
  
  try {
      drawbotReady = false;
      drawbotConnected = false;
      //serialPortNumber = newSerialPort;
      networkHost = newNetworkHost;
     
      //myPort = new Serial(this, portName, getBaudRate());
      //myPort = new Client(this, "192.168.1.24", 12345);
      myPort = new Client(this, newNetworkHost, 12345);
    
      if ( myPort.active() ) {       
        //read bytes into a buffer until you get a linefeed (ASCII 10):
        //myPort.bufferUntil('\n');
        myPort.readBytesUntil('\n');          
        useSerialPortConnection = true;
        println("Successfully connected to network host " + newNetworkHost);
      } else {
        println("Attempting to connect to network host " + newNetworkHost + " caused an exception" );
      }
    } 
    catch (Exception e) {
      // so... you can't catch a client exception!?!
        println("Attempting to connect to network host " + newNetworkHost 
          + " caused an exception: " + e.getMessage());
    } 
}

