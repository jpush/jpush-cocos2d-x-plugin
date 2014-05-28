import org.json.JSONException;
import org.json.JSONObject;
import cn.jpush.android.api.JPushInterface;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

public class JPushReceiver extends BroadcastReceiver{
	public static native void didReceiveRemoteNotification(String message);

	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		Bundle bundle = intent.getExtras();
	
		JSONObject object = new JSONObject();
		try {
			if (JPushInterface.ACTION_REGISTRATION_ID.equals(intent.getAction())) {
				object.put("registrationId", bundle.getString(JPushInterface.EXTRA_REGISTRATION_ID));
		    } else if (JPushInterface.ACTION_MESSAGE_RECEIVED.equals(intent.getAction())) {
		    	object.put("extraTitle", bundle.getString(JPushInterface.EXTRA_TITLE));
				object.put("extraMessage", bundle.getString(JPushInterface.EXTRA_MESSAGE));
				object.put("extraExtra", bundle.getString(JPushInterface.EXTRA_EXTRA));
				object.put("extraType", bundle.getString(JPushInterface.EXTRA_CONTENT_TYPE));
				object.put("richPushFilePath", bundle.getString(JPushInterface.EXTRA_RICHPUSH_FILE_PATH));
				object.put("extraMsgId", bundle.getString(JPushInterface.EXTRA_MSG_ID));
		    } else if (JPushInterface.ACTION_NOTIFICATION_RECEIVED.equals(intent.getAction())) {
		        object.put("extraNotificationTitle", bundle.getString(JPushInterface.EXTRA_NOTIFICATION_TITLE));
		        object.put("extraAlert", bundle.getString(JPushInterface.EXTRA_ALERT));
		        object.put("extraExtra", bundle.getString(JPushInterface.EXTRA_EXTRA));
		        object.put("extraNotificationId", bundle.getInt(JPushInterface.EXTRA_NOTIFICATION_ID));
		        object.put("extraContentTyoe", bundle.getString(JPushInterface.EXTRA_CONTENT_TYPE));
		        object.put("extraRichPushHtmlPath", bundle.getString(JPushInterface.EXTRA_RICHPUSH_HTML_PATH));
		        String fileStr = bundle.getString(JPushInterface.EXTRA_RICHPUSH_HTML_RES);
		        if (fileStr!=null) {
			        String[] fileNames = fileStr.split(",");
			        object.put("extraRichPushHtmlRes", fileNames);
				}
		        object.put("extraMsgId", bundle.getString(JPushInterface.EXTRA_MSG_ID));
		    } else if (JPushInterface.ACTION_NOTIFICATION_OPENED.equals(intent.getAction())) {
		        object.put("extraNotificationTitle", bundle.getString(JPushInterface.EXTRA_NOTIFICATION_TITLE));
		        object.put("extraAlert", bundle.getString(JPushInterface.EXTRA_ALERT));
		        object.put("extraExtra", bundle.getString(JPushInterface.EXTRA_EXTRA));
		        object.put("extraNotificationId", bundle.getInt(JPushInterface.EXTRA_NOTIFICATION_ID));
		        object.put("extraMsgId", bundle.getString(JPushInterface.EXTRA_MSG_ID));
		    } else {
		       
		    }
		} catch (JSONException e) {
			// TODO: handle exception
		}
		String jsonString = object.toString();
        didReceiveRemoteNotification(jsonString);
	}

}
