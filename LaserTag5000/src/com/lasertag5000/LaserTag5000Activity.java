package com.lasertag5000;

import org.apache.http.HttpException;
import org.apache.http.HttpHost;
import org.apache.http.HttpRequest;
import org.apache.http.HttpRequestInterceptor;
import org.apache.http.HttpResponse;
import org.apache.http.auth.AuthScope;
import org.apache.http.auth.AuthState;
import org.apache.http.auth.Credentials;
import org.apache.http.auth.UsernamePasswordCredentials;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.CredentialsProvider;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.protocol.ClientContext;
import org.apache.http.impl.auth.BasicScheme;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.protocol.ExecutionContext;
import org.apache.http.protocol.HttpContext;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.*;
import android.widget.CompoundButton.OnCheckedChangeListener;
import com.lasertag5000.LaserTagService;

public class LaserTag5000Activity extends Activity {
	
    private class LoginTask extends AsyncTask<String, String, String>
    {
    	protected String doInBackground(String... strings)
    	{
    		try
    		{
	    		DefaultHttpClient client = new DefaultHttpClient();
	    		
	    		HttpRequestInterceptor preemptiveAuth = new HttpRequestInterceptor()
	    		{
	    			public void process(final HttpRequest request, final HttpContext context) throws HttpException, IOException 
	    			{
	    				AuthState authState = (AuthState) context.getAttribute(ClientContext.TARGET_AUTH_STATE);
	    				CredentialsProvider credsProvider = (CredentialsProvider) context.getAttribute(ClientContext.CREDS_PROVIDER);
	    				HttpHost targetHost = (HttpHost) context.getAttribute(ExecutionContext.HTTP_TARGET_HOST);
	    				
	    				if(authState.getAuthScheme() == null)
	    				{
	    					AuthScope scope = new AuthScope(targetHost.getHostName(), targetHost.getPort());
	    					Credentials creds = credsProvider.getCredentials(scope);
	    					if(creds != null)
	    					{
	    						authState.setAuthScheme(new BasicScheme());
	    						authState.setCredentials(creds);
	    					}
	    				}
	    			}
	    		};
	    		
	    		client.addRequestInterceptor(preemptiveAuth, 0);
	    		Credentials creds = new UsernamePasswordCredentials(strings[0], strings[1]);
	    		client.getCredentialsProvider().setCredentials(new AuthScope(AuthScope.ANY_HOST ,AuthScope.ANY_PORT,AuthScope.ANY_REALM), creds);
	    		HttpGet request = new HttpGet();
	    		request.setURI(new URI(getString(R.string.url)));
	    		HttpResponse response = client.execute(request);
	    		if(response.getStatusLine().getStatusCode() == 401) return("Authentication Failed!");
	    		if(response.getStatusLine().getStatusCode() != 200) return("HTTP Error!");
    		} 
    		catch (URISyntaxException e) 
    		{
				return("Failed to parse URL");
			} catch (ClientProtocolException e) {
				return(e.getMessage());
			} catch (IOException e) {
				return(e.getMessage());
			}
    		
    		return("");
    	}
    	
    	protected void onPostExecute(String result)
    	{
    		if(result == "")
    		{
    			Toast.makeText(LaserTag5000Activity.this, "Success", Toast.LENGTH_SHORT).show();
    			Intent newIntent = new Intent(LaserTag5000Activity.this, MainActivity.class);
    			startActivityForResult(newIntent, 0);
    		}
    		else
    		{
    			Toast.makeText(LaserTag5000Activity.this, result, Toast.LENGTH_LONG).show();
    		}
    	}
    }
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login);
        
        Log.v("LASERS", "onClick: starting srvice");
        
        CheckBox showpassword = (CheckBox) findViewById(R.id.logon_showpassword);
        showpassword.setOnCheckedChangeListener(new OnCheckedChangeListener() 
        {
        	public void onCheckedChanged(CompoundButton button, boolean checked)
        	{
        		EditText password = (EditText) findViewById(R.id.login_password);
        		
        		if(checked)
        		{
        			password.setInputType(android.text.InputType.TYPE_CLASS_TEXT | android.text.InputType.TYPE_TEXT_VARIATION_VISIBLE_PASSWORD);
        		}
        		else
        		{
        			password.setInputType(android.text.InputType.TYPE_CLASS_TEXT | android.text.InputType.TYPE_TEXT_VARIATION_PASSWORD);
        		}
        	}
        });
        
        Button loginbutton = (Button) findViewById(R.id.logon_button);
        loginbutton.setOnClickListener(new View.OnClickListener()
        {
        	public void onClick(View v)
        	{
        		EditText username = (EditText) findViewById(R.id.login_username);
        		EditText password = (EditText) findViewById(R.id.login_password);
        		
        		if((username.getText().toString().equals("")) || (password.getText().toString().equals("")))
        		{
        			Toast.makeText(LaserTag5000Activity.this, "You must provide a username and password.", Toast.LENGTH_SHORT).show();
        		}
        		else
        		{
        			new LoginTask().execute(username.getText().toString(), password.getText().toString());
        		}
        	}
        	
        });
    }
}
