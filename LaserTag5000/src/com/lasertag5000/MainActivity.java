package com.lasertag5000;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	public LaserTagService boundService;
	public BluetoothAdapter adapter = null;
	public BluetoothSocket socket = null;
	public InputStream instream = null;
	public OutputStream outstream = null;
	public TextView commandview = null;
	
	public int health = 100;
	public int ammo = 10;
	
	public static final UUID SPP_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	public static final String SPP_ADDRESS = "00:06:66:43:45:75";
	static final String[] sounds = new String[] {
		"Mil Shot",
		"Empty",
		"Clip",
		"Milaction",
		"Near Miss",
		"Damage",
		"Dead",
		"Powerup",
		"Beep",
		"Buzz",
		"Add Health",
		"Add Ammo",
		"Quak!",
		"Game Over",
		"Explosion",
		"Unused",
		"Flag",
		"Flag Score",
		"Clone Ok",
		"Sensor Fail",
		"Sci Shot",
		"Sci Action",
		"Score Ok",
		"Unused",
		"Silenced",
		"Disarm",
		"Low Batt",
		"Unused",
		"Unused",
		"Stunned"
	};

	class ReadTask extends AsyncTask<Void, String, Void>
	{
		@Override
		protected Void doInBackground(Void... arg0) {
			
			BufferedReader reader = new BufferedReader(new InputStreamReader(MainActivity.this.instream));			
			
			while(true)
			{
				try {
					publishProgress(reader.readLine());
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		
		protected void onProgressUpdate(String... line)
		{
			log(line[0]);
			MainActivity.this.parseReceived(line[0].split(" "));
		}
	};
	
	public void log(String msg)
	{
		commandview.setText(msg + "\n" + commandview.getText());
	}
	
	public void sendCommand(String cmd)
	{
		cmd = cmd + "\n";
		try
        {
        	outstream.write(cmd.getBytes());
        }
        catch(IOException e)
        {
        	Toast.makeText(this, "Failed to send " + cmd, Toast.LENGTH_LONG).show();
        	return;
        }
		
		log(cmd);
	}
	
	public void parseReceived(String[] cmd)
	{
		//skip blank commands
		if(cmd.length == 0) return;
		
		if(cmd[0].equals("A"))		//ACK
		{
			if(cmd.length == 1) return; //ignore useless lines
			onAck(cmd);
		}
		else if(cmd[0].equals("I")) //Event
		{
			if(cmd.length == 1) return; //ignore useless lines
			onEvent(cmd);
		}
		else if(cmd[0].equals("E")) //Error
		{
			if(cmd.length == 1) return; //ignore useless lines
			onError(cmd);
		}
		else if(cmd[0].equals("A")) //Debug
		{
			if(cmd.length == 1) return; //ignore useless lines
			onDebug(cmd);
		}
	}
	
	public void onAck(String[] cmd)
	{
		
	}
	
	public void onEvent(String[] cmd)
	{
		if(cmd[1].equals("TRIGGER_PRESS"))
		{
			if(ammo == 0)
			{
				sendCommand("p 1");
			}
			else
			{
				sendCommand("p 0");
				sendCommand("F");
				ammo--;
				setAmmo(ammo);
			}
		}
		else if(cmd[1].equals("RELOAD_PRESS"))
		{
			sendCommand("p 3");
			ammo = 10;
			if(health <= 0) health = 100;
			
			setAmmo(ammo);
			setHealth(health);
		}
		else if(cmd[1].equals("RECEIVED_IR"))
		{
			health-=8;
			setHealth(health);
			if(health <= 0)
			{
				sendCommand("p 6");
			}
			else
			{
				sendCommand("p 5");
				
			}
		}
	}
	
	public void onError(String[] cmd)
	{
		
	}
	
	public void onDebug(String[] cmd)
	{
		
	}
	
	void setHealth(int health)
	{
		int tmp = health;
		if(tmp < 0) tmp = 0;
		
		TextView text = (TextView)findViewById(R.id.health);
		text.setText(Integer.toString(tmp));
		
		ProgressBar progress = (ProgressBar)findViewById(R.id.healthprogress);
		progress.setProgress(tmp);
	}
	
	void setAmmo(int ammo)
	{
		int tmp = ammo;
		if(tmp < 0) tmp = 0;
		
		TextView text = (TextView)findViewById(R.id.ammo);
		text.setText(Integer.toString(tmp));
		
		ProgressBar progress = (ProgressBar)findViewById(R.id.ammoprogress);
		progress.setProgress(tmp);
	}
	
	@Override
    public void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        TabHost th = (TabHost)findViewById(R.id.tabhost);
        
        startService(new Intent(this, LaserTagService.class));
        th.setup();
        
        LayoutInflater.from(this).inflate(R.layout.statstab, th.getTabContentView(), true);
        TabSpec tab1 = th.newTabSpec("tab1");
        tab1.setContent(R.id.statstab);
        tab1.setIndicator("Stats");
        th.addTab(tab1);
        
        LayoutInflater.from(this).inflate(R.layout.soundstab, th.getTabContentView(), true);
        TabSpec tab2 = th.newTabSpec("tab2");
        tab2.setContent(R.id.soundstab);
        tab2.setIndicator("Sounds");
        th.addTab(tab2);
        
        LayoutInflater.from(this).inflate(R.layout.consoletab, th.getTabContentView(), true);
        TabSpec tab3 = th.newTabSpec("tag3");
        tab3.setContent(R.id.consoletab);
        tab3.setIndicator("Console");
        th.addTab(tab3);
        
        ListView soundlist = (ListView)findViewById(R.id.soundstab);
        soundlist.setAdapter(new ArrayAdapter<String>(this, R.layout.layout_item_text, sounds));
      
        soundlist.setOnItemClickListener(new OnItemClickListener() {
        	public void onItemClick(AdapterView<?> parent, View view, int position, long id)
        	{
        		sendCommand("p " + Integer.toString(position));
        	}
        });
        
        commandview = (TextView)findViewById(R.id.consoletext);
        
        Button sendbutton = (Button) findViewById(R.id.sendbutton);
        sendbutton.setOnClickListener(new View.OnClickListener()
        {
        	public void onClick(View v)
        	{
        		EditText command = (EditText) findViewById(R.id.commandtext);
        		sendCommand(command.getText().toString());
        		command.setText("");
        	}
        	
        });
        
        setHealth(health);
        setAmmo(ammo);
        
        //Bluetooth
        adapter = BluetoothAdapter.getDefaultAdapter();
        if(adapter == null)
        {
        	Toast.makeText(this, "No Bluetooth Adapter Found!", Toast.LENGTH_LONG).show();
        	finish();
        	return;
        }
        
        if(!adapter.isEnabled())
        {
        	Toast.makeText(this, "Bluetooth is not enabled!", Toast.LENGTH_LONG).show();
        	finish();
        	return;
        }
        
        BluetoothDevice device = adapter.getRemoteDevice(SPP_ADDRESS);
        
        try
        {
        	socket = device.createRfcommSocketToServiceRecord(SPP_UUID);
        }
        catch(IOException e)
        {
        	Toast.makeText(this, "Failed to create bluetooth socket!", Toast.LENGTH_LONG).show();
        	finish();
        	return;
        }
        
        //Stop discovery if it's already going on
        adapter.cancelDiscovery();
        
        try
        {
        	socket.connect();
        }
        catch(IOException e)
        {
        	try
        	{
        		socket.close();
        	}
        	catch(IOException e2)
        	{
        		//don't care
        	}
        	
        	Toast.makeText(this, "Failed to connect to bluetooth device!", Toast.LENGTH_LONG).show();
        	finish();
        	return;
        }
        
        try
        {
        	outstream = socket.getOutputStream();
        	instream = socket.getInputStream();
        }
        catch(IOException e)
        {
        	Toast.makeText(this, "Failed to get streams!", Toast.LENGTH_LONG).show();
        	finish();
        	return;
        }
        
        new ReadTask().execute();
        sendCommand("H");
	}

	@Override
	public void onDestroy()
	{
		super.onDestroy();

		if(outstream != null)
		{
			try 
			{
				outstream.flush();
			}
			catch(IOException e)
			{
				//don't care
			}
		}
		
		try
		{
			socket.close();
		}
		catch(IOException e)
		{
			//don't care
		}
	}
	
    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
    	MenuInflater inflater = getMenuInflater();
    	inflater.inflate(R.menu.login_menu, menu);
    	return(true);
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
    	switch(item.getItemId())
    	{
    	case R.id.login_stopservice:
    		stopService(new Intent(this, LaserTagService.class));
    		break;
    	}
    	
    	return(true);
    }
}
