import java.util.Set;

import android.content.Context;
import cn.jpush.android.api.JPushInterface;
import cn.jpush.android.api.TagAliasCallback;
public class JPushCallbackHelper {
	
	JPushCallbackHelper(){		
	}
	private long callback_ptr;
	private long callback_handler;

	public static native void setAliasAndTagsCallback(long fun_handler,int resultCode, String alias,Set<String> tags,long func_ptr);
		
	public void setAliasAndTags(long func_handler,Context context, String alias, Set<String> tags,long func_ptr) {
		callback_ptr = func_ptr;
		callback_handler = func_handler;
		JPushInterface.setAliasAndTags(context, alias, tags, new TagAliasCallback() {
			@Override
			public void gotResult(int arg0, String arg1, Set<String> arg2) {
				// TODO Auto-generated method stub
				setAliasAndTagsCallback(callback_handler,arg0, arg1, arg2, callback_ptr);				
			}
		});
	}
	
	public void setAlias(long func_handler,Context context,String alias,long func_ptr){
		callback_ptr = func_ptr;
		callback_handler = func_handler;
		JPushInterface.setAlias(context, alias, new TagAliasCallback() {
			@Override
			public void gotResult(int arg0, String arg1, Set<String> arg2) {
				// TODO Auto-generated method stub
				setAliasAndTagsCallback(callback_handler,arg0, arg1, arg2, callback_ptr);			
			}
		});
	}
	
	public void setTags(long func_handler,Context context,Set<String> tags,long func_ptr){
		callback_ptr = func_ptr;
		callback_handler = func_handler;
		JPushInterface.setTags(context, tags, new TagAliasCallback() {
			@Override
			public void gotResult(int arg0, String arg1, Set<String> arg2) {
				// TODO Auto-generated method stub
				setAliasAndTagsCallback(callback_handler,arg0, arg1, arg2, callback_ptr);			
			}
		});
		
	}
	
}
