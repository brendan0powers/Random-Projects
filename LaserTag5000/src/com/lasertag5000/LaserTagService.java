package com.lasertag5000;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Binder;
import android.os.IBinder;
import java.lang.Runnable;
import java.net.MalformedURLException;
import java.net.URL;

import org.ektorp.CouchDbConnector;
import org.ektorp.CouchDbInstance;
import org.ektorp.android.http.AndroidHttpClient;
import org.ektorp.http.HttpClient;
import org.ektorp.impl.StdCouchDbConnector;
import org.ektorp.impl.StdCouchDbInstance;

import com.couchbase.android.*;
import com.lasertag5000.LaserTag5000Activity;

public class LaserTagService extends Service {
	private NotificationManager nmgr;
	private int NOTIFICATION = R.string.ServiceString;
	
	private Runnable serviceThreadRunnable = new Runnable() {
		private String dbHost;
		private int dbPort;
		private CouchDbInstance dbInstance;
		private CouchDbConnector db;
		private AndroidHttpClient httpClient;
		
		private ICouchbaseDelegate couchdelegate = new ICouchbaseDelegate()
		{
			public void couchbaseStarted(String host, int port) 
			{
				/*
				dbHost = host;
				dbPort = port;
				
				try {
					httpClient = (AndroidHttpClient) new AndroidHttpClient.Builder()
													.username("brendan0powers")
													.password("4949394")
													.url("http://brendan0powers.iriscouch.com:80/").build();
				} catch (MalformedURLException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
												//.host(host)
												//.port(port)
												//.build();

				dbInstance = new StdCouchDbInstance(httpClient);
				db = dbInstance.createConnector("lasertag",true);

				db.createDatabaseIfNotExists();
				*/
			}
			
			public void exit(String error) 
			{
				
			}
		};
		
		private ServiceConnection couchService;
		
		public void startCouch()
		{
			//CouchbaseMobile couch = new CouchbaseMobile(getBaseContext(), couchdelegate);
			//couchService = couch.startCouchbase();
		}
		
		public void run()
		{
			startCouch();
		}
	};
	private Thread serviceThread = new Thread(null, serviceThreadRunnable, "service_thread");
	
	public class LocalBinder extends Binder
	{
		LaserTagService getService()
		{
			return LaserTagService.this;
		}
	}
	private final IBinder mBinder = new LocalBinder();
	
	@Override
	public IBinder onBind(Intent arg0) {
		return mBinder;
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		return START_STICKY;
	}
	
	@Override
	public void onCreate()
	{
		nmgr = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
		
		//Create the notification
		CharSequence text = getText(R.string.ServiceString);
		Notification notification = new Notification(R.drawable.icon, text, System.currentTimeMillis());
		PendingIntent intent = PendingIntent.getActivity(this, 0, new Intent(this, MainActivity.class), 0);
		notification.setLatestEventInfo(this, getText(R.string.app_name), text, intent);
		notification.flags |= Notification.FLAG_NO_CLEAR | Notification.FLAG_ONGOING_EVENT;
		nmgr.notify(NOTIFICATION, notification);
		
		serviceThread.start();
	}
	
	@Override
	public void onDestroy()
	{
		serviceThread.stop();
		nmgr.cancel(NOTIFICATION);
	}
}
