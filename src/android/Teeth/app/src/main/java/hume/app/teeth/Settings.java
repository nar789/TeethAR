package hume.app.teeth;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

/**
 * Created by leejunghun on 2018-03-01.
 */

public class Settings extends AppCompatActivity {

    private Button btn;
    private EditText ip;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        btn=(Button)this.findViewById(R.id.setbtn);
        ip=(EditText)this.findViewById(R.id.ipaddr);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String ipaddr=ip.getText().toString();
                Intent intent=new Intent(Settings.this,MainActivity.class);
                intent.putExtra("ipaddr",ipaddr);
                startActivity(intent);
            }
        });
    }
}
