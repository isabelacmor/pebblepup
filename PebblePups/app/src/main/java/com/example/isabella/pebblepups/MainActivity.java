package com.example.isabella.pebblepups;

import android.app.ActionBar;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.os.Handler;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.UUID;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.PebbleKit.PebbleDataReceiver;
import com.getpebble.android.kit.util.PebbleDictionary;


public class MainActivity extends ActionBarActivity {

    private SharedPreferences sharedpreferences;
    private static final String PEBBLE_APP_UUID = "945fcb0e-31fa-4dc2-a96d-a441e6ae6cc1";
    private static final String DEFAULT_DOG_NAME = "poop";
    private static final int DEFAULT_DOG_GENDER = 1;

    private static final UUID WATCHAPP_UUID = UUID.fromString(PEBBLE_APP_UUID);
    private static final String WATCHAPP_FILENAME = "pebble.pbw";

    private static final int
            KEY_BUTTON = 0,
            KEY_UP = 1,
            KEY_DOWN = 2,
            KEY_LEFT = 3,
            KEY_RIGHT = 4,
            KEY_UP_LEFT = 5,
            KEY_UP_RIGHT = 6,
            KEY_DOWN_LEFT = 7,
            KEY_DOWN_RIGHT = 8,
            KEY_UPDATE_NAME = 9,
            KEY_UPDATE_BOTH = 10,
            KEY_SHOW_OFF = 11,
            BUTTON_UP = 0,
            BUTTON_SELECT = 1,
            BUTTON_DOWN = 2;

    private Handler handler = new Handler();
    private PebbleDataReceiver appMessageReciever;
    private TextView whichButtonView;

    private static boolean movingUp = false, movingDown = false, movingLeft = false, movingRight = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Open stored settings
        sharedpreferences = getSharedPreferences("PebblePupPrefs", Context.MODE_PRIVATE);

        // Open Pebble Companion App
        PebbleKit.startAppOnPebble(getApplicationContext(), UUID.fromString(PEBBLE_APP_UUID));

        // Update data on the Pebble App
        String allSettings = sharedpreferences.getString("SETTINGS", ";");
        String[] both = allSettings.split(";");

        if(both.length == 0) {
            updateSettings(DEFAULT_DOG_NAME, DEFAULT_DOG_GENDER, getApplicationContext());
        } else if(both.length == 1) {
            updateSettings(DEFAULT_DOG_NAME, both[0].equalsIgnoreCase("BOY") ? 0 : 1, getApplicationContext());
        } else {
            updateSettings(both[1], both[0].equalsIgnoreCase("BOY") ? 0 : 1, getApplicationContext());
        }

        // Add listeners to all directional buttons
        FloatingActionButton moveUpButton = (FloatingActionButton)findViewById(R.id.button_move_up);
        FloatingActionButton moveDownButton = (FloatingActionButton)findViewById(R.id.button_move_down);
        FloatingActionButton moveLeftButton = (FloatingActionButton)findViewById(R.id.button_move_left);
        FloatingActionButton moveRightButton = (FloatingActionButton)findViewById(R.id.button_move_right);
        Button showOffButton = (Button)findViewById(R.id.button_show_off);

        moveUpButton.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN ) {
                    // Pressing the button
                    movingUp = true;
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP ) {
                    // Releasing the button
                    movingUp = false;
                    return true;
                }

                doMove(getApplicationContext());
                return false;
            }
        });

        moveDownButton.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN ) {
                    // Pressing the button
                    movingDown = true;
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP ) {
                    // Releasing the button
                    movingDown = false;
                    return true;
                }

                doMove(getApplicationContext());
                return false;
            }
        });

        moveLeftButton.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN ) {
                    // Pressing the button
                    movingLeft = true;
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP ) {
                    // Releasing the button
                    movingLeft = false;
                    return true;
                }

                doMove(getApplicationContext());
                return false;
            }
        });

        moveRightButton.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    // Pressing the button
                    movingRight = true;
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    // Releasing the button
                    movingRight = false;
                    return true;
                }

                doMove(getApplicationContext());
                return false;
            }
        });

        showOffButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // Send show off command to Pebble
                PebbleDictionary out = new PebbleDictionary();
                out.addString(KEY_SHOW_OFF, "DO");
                //out.addString(KEY_UPDATE_NAME, name);
                PebbleKit.sendDataToPebble(getApplicationContext(), WATCHAPP_UUID, out);

            }
        });

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            // custom dialog
            final Dialog dialog = new Dialog(MainActivity.this);
            dialog.setContentView(R.layout.settings);
            dialog.setTitle("Pebble Pups Settings");

            // Get the current saved settings
            String allSettings = sharedpreferences.getString("SETTINGS", ";");
            final String[] both = allSettings.split(";");

            // Set the text field based on stored value
            final EditText mEdit = (EditText) dialog.findViewById(R.id.dog_name);
            mEdit.setText(both.length >= 2 ? both[1] : "", TextView.BufferType.EDITABLE);

            final RadioGroup genders = (RadioGroup) dialog.findViewById(R.id.gender_choices);

            // Set the checked radio button based on stored value
            int count = genders.getChildCount();
            for (int i=0;i<count;i++) {
                View o = genders.getChildAt(i);
                if (o instanceof RadioButton) {
                    if(i == (both[0].equalsIgnoreCase("BOY") ? 0 : 1)) {
                        ((RadioButton) o).setChecked(true);
                    } else {
                        ((RadioButton) o).setChecked(false);
                    }
                }
            }

            Button dialogButton = (Button) dialog.findViewById(R.id.dialogButtonOK);
            // if button is clicked, close the custom dialog
            dialogButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {

                    int radioButtonID = genders.getCheckedRadioButtonId();
                    View radioButton = genders.findViewById(radioButtonID);
                    int index = genders.indexOfChild(radioButton);

                    if((index == 1 && both[0].equalsIgnoreCase("GIRL")) || (index == 0 && both[0].equalsIgnoreCase("BOY"))) {
                        System.out.println("Sending only name");
                        updateSettings(mEdit.getText().toString(), MainActivity.this);
                    } else {
                        System.out.println("Sending both name and gender");
                        updateSettings(mEdit.getText().toString(), index, MainActivity.this);
                    }

                    dialog.dismiss();
                }
            });

            dialog.show();
        }

        return super.onOptionsItemSelected(item);
    }

    public void updateSettings(String name, Context context) {
        // Send name to Pebble
        PebbleDictionary out = new PebbleDictionary();
        out.addString(KEY_UPDATE_NAME, name);
        PebbleKit.sendDataToPebble(context, WATCHAPP_UUID, out);

        // Save in local storage to repopulate next time
        String allSettings = sharedpreferences.getString("SETTINGS", ";");
        String[] both = allSettings.split(";");

        SharedPreferences.Editor editor = sharedpreferences.edit();
        editor.putString("SETTINGS", both[0] + ";" + name);
        editor.commit();
    }

    public void updateSettings(String name, int gender, Context context) {
        System.out.println("UPDATING TO: " + (gender == 0 ? "BOY;"+name : "GIRL;"+name));

        // Send name to Pebble
        PebbleDictionary out = new PebbleDictionary();
        out.addString(KEY_UPDATE_BOTH, (gender == 0 ? "BOY;"+name : "GIRL;"+name));
        //out.addString(KEY_UPDATE_NAME, name);
        PebbleKit.sendDataToPebble(context, WATCHAPP_UUID, out);

        // Save in local storage to repopulate next time
        SharedPreferences.Editor editor = sharedpreferences.edit();
        editor.putString("SETTINGS", (gender == 0 ? "BOY" : "GIRL") + ";" + name);
        editor.commit();
    }

    @Override
    protected void onResume() {
        super.onResume();

        // Define AppMessage behavior
        if(appMessageReciever == null) {
            appMessageReciever = new PebbleDataReceiver(WATCHAPP_UUID) {

                @Override
                public void receiveData(Context context, int transactionId, PebbleDictionary data) {
                    // Always ACK
                    PebbleKit.sendAckToPebble(context, transactionId);

                    // What message was received?
                    if(data.getInteger(KEY_BUTTON) != null) {

                        // Update data on the Pebble App

                        // KEY_BUTTON was received, determine which button
//                        final int button = data.getInteger(KEY_BUTTON).intValue();

//                        // Update UI on correct thread
//                        handler.post(new Runnable() {
//
//                            @Override
//                            public void run() {
//                                switch(button) {
//                                    case BUTTON_UP:
//                                        whichButtonView.setText("UP");
//                                        break;
//                                    case BUTTON_SELECT:
//                                        whichButtonView.setText("SELECT");
//                                        break;
//                                    case BUTTON_DOWN:
//                                        whichButtonView.setText("DOWN");
//                                        break;
//                                    default:
//                                        Toast.makeText(getApplicationContext(), "Unknown button: " + button, Toast.LENGTH_SHORT).show();
//                                        break;
//                                }
//                            }
//
//                        });
                    }
                }
            };

            // Add AppMessage capabilities
            PebbleKit.registerReceivedDataHandler(this, appMessageReciever);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();

        // Unregister AppMessage reception
        if(appMessageReciever != null) {
            unregisterReceiver(appMessageReciever);
            appMessageReciever = null;
        }
    }

    /**
     * Alternative sideloading method
     * Source: http://forums.getpebble.com/discussion/comment/103733/#Comment_103733
     */
    public static void sideloadInstall(Context ctx, String assetFilename) {
        try {
            // Read .pbw from assets/
            Intent intent = new Intent(Intent.ACTION_VIEW);
            File file = new File(ctx.getExternalFilesDir(null), assetFilename);
            InputStream is = ctx.getResources().getAssets().open(assetFilename);
            OutputStream os = new FileOutputStream(file);
            byte[] pbw = new byte[is.available()];
            is.read(pbw);
            os.write(pbw);
            is.close();
            os.close();

            // Install via Pebble Android app
            intent.setDataAndType(Uri.fromFile(file), "application/pbw");
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            ctx.startActivity(intent);
        } catch (IOException e) {
            Toast.makeText(ctx, "App install failed: " + e.getLocalizedMessage(), Toast.LENGTH_LONG).show();
        }
    }

    public static void doMove(Context context) {
        PebbleDictionary out = new PebbleDictionary();

        if(movingUp) {
            if(movingLeft) {
                System.out.println("moving U + L");
                out.addString(KEY_UP_LEFT, "U+L");
            } else if(movingRight) {
                System.out.println("moving U + R");
                out.addString(KEY_UP_RIGHT, "U+R");
            } else {
                System.out.println("moving U");
                out.addString(KEY_UP, "U");
            }
        } else if(movingDown) {
            if(movingLeft) {
                System.out.println("moving D + L");
                out.addString(KEY_DOWN_LEFT, "D+L");
            } else if(movingRight) {
                System.out.println("moving D + R");
                out.addString(KEY_DOWN_RIGHT, "D+R");
            } else {
                System.out.println("moving D");
                out.addString(KEY_DOWN, "D");
            }
        } else if(movingLeft) {
            System.out.println("moving L");
            out.addString(KEY_LEFT, "L");
        } else if(movingRight) {
            System.out.println("moving R");
            out.addString(KEY_RIGHT, "R");
        }

        PebbleKit.sendDataToPebble(context, WATCHAPP_UUID, out);
    }
}
