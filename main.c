#include "globals.h"
#include "alignments.h"
#include "encodeUtils.h"
#include "file.h"
#include <locale.h>
#include <gtk/gtk.h>

GtkWidget *entrada, *nw, *sw;
GtkEntry *h1, *h2, *cantThreads, *match, *miss, *gap, *valueK, *ampB, *valueg, *valueh, *sw_valueg, *sw_valueh;
GtkCheckButton *v_left, *v_right, *w_left, *w_right;
GtkCheckButton *nw_k_band, *nw_blocks, *sw_blocks;
GtkComboBoxText *comboBox, *nw_type, *sw_type;
GtkBuilder *builder;

int k_init_value;
int k_amp_value;
char *string_v, *string_w;

void setInitialValues(){
    gtk_entry_set_text (match, "1");
    gtk_entry_set_text (miss, "-1");
    gtk_entry_set_text (gap, "-2");
    gtk_entry_set_text (cantThreads, "4");
}

void getGtkWidgets(GtkBuilder *builder) {
    entrada = GTK_WIDGET(gtk_builder_get_object(builder, "entrada"));
    cantThreads = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "threads"));
    h1 = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "h1"));
    h2 = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "h2"));
    match = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "match"));
    miss = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "miss"));
    gap = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "gap"));
    comboBox = (GtkComboBoxText *) GTK_WIDGET(gtk_builder_get_object(builder, "alignment"));

    nw = GTK_WIDGET(gtk_builder_get_object(builder, "nw"));
    v_left = (GtkCheckButton *) GTK_WIDGET(gtk_builder_get_object(builder, "v_left"));
    v_right = (GtkCheckButton *) GTK_WIDGET(gtk_builder_get_object(builder, "v_right"));
    w_left = (GtkCheckButton *) GTK_WIDGET(gtk_builder_get_object(builder, "w_left"));
    w_right = (GtkCheckButton *) GTK_WIDGET(gtk_builder_get_object(builder, "w_right"));
    nw_k_band = (GtkCheckButton *)  GTK_WIDGET(gtk_builder_get_object(builder, "nw_k_band"));
    nw_blocks = (GtkCheckButton *)  GTK_WIDGET(gtk_builder_get_object(builder, "nw_blocks"));
    nw_type = (GtkComboBoxText *) GTK_WIDGET(gtk_builder_get_object(builder, "nw_type"));
    valueK = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "valueK"));
    ampB = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "ampB"));
    valueg = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "valueg"));
    valueh = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "valueh"));

    sw =  GTK_WIDGET(gtk_builder_get_object(builder, "sw"));
    sw_blocks = (GtkCheckButton *)  GTK_WIDGET(gtk_builder_get_object(builder, "sw_blocks"));
    sw_valueg = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "sw_valueg"));
    sw_valueh = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "sw_valueh"));
    sw_type = (GtkComboBoxText *) GTK_WIDGET(gtk_builder_get_object(builder, "sw_type"));

    setInitialValues();
}

int getType(GtkCheckButton *left, GtkCheckButton *right){
    bool left_active = gtk_toggle_button_get_active(left);
    bool right_active = gtk_toggle_button_get_active(right);
    if(!left_active && !right_active){
        return penalty_left_penalty_right;
    } else if(!left_active && right_active){
        return penalty_left_free_right;
    } else if(left_active && !right_active){
        return free_left_penalty_right;
    } else if(left_active && right_active){
        return free_left_free_right;
    }
}


void on_continueSW_clicked(GtkButton *button, gpointer data){
    char* selected = gtk_combo_box_text_get_active_text(sw_type);
    if(selected[0] == '2'){
        char *temp = encodeOneLetter(string_v);
        string_v = temp;
        char *temp2 = encodeOneLetter(string_w);
        string_w = temp2;
    } else if(selected[0] == '3'){
        char *temp = encodeDnaToProtein(string_v);
        string_v = temp;
    } else if(selected[0] == '4'){
        char *temp = encodeDnaToProtein(string_v);
        string_v = temp;
    }  else if(selected[0] == '7'){
        char *temp2 = encodeOneLetter(string_w);
        string_w = temp2;
    }

    if(gtk_toggle_button_get_active(sw_blocks)){
        score_table.new_block_cost = atoi(gtk_entry_get_text(sw_valueg));
        score_table.continue_block_cost = atoi(gtk_entry_get_text(sw_valueh));
        runSmithWaterman(string_v, string_w, gap_blocks, threads);
    } else {
        runSmithWaterman(string_v, string_w, none, threads);
    }
}


void on_continueNW_clicked(GtkButton *button, gpointer data){
    int type_v = getType(v_left, v_right);
    int type_w = getType(w_left, w_right);

    char* selected = gtk_combo_box_text_get_active_text(nw_type);
    if(selected[0] == '2'){
        char *temp = encodeOneLetter(string_v);
        string_v = temp;
        char *temp2 = encodeOneLetter(string_w);
        string_w = temp2;
    } else if(selected[0] == '3'){
        char *temp = encodeDnaToProtein(string_v);
        string_v = temp;
    } else if(selected[0] == '4'){
        char *temp = encodeDnaToProtein(string_v);
        string_v = temp;
    }  else if(selected[0] == '7'){
        char *temp2 = encodeOneLetter(string_w);
        string_w = temp2;
    }

    if(gtk_toggle_button_get_active(nw_k_band)){
        k_init_value = atoi(gtk_entry_get_text (valueK));
        k_amp_value = atoi(gtk_entry_get_text (ampB));
        runNeedlemanWunsch(type_v, type_w, string_v, string_w, k_band, threads, k_init_value, k_amp_value);
    } else if(gtk_toggle_button_get_active(nw_blocks)){
      printf("here nw\n");
        score_table.new_block_cost = atoi(gtk_entry_get_text(valueg));
        score_table.continue_block_cost = atoi(gtk_entry_get_text(valueh));
        runNeedlemanWunsch(type_v, type_w, string_v, string_w, gap_blocks, threads, 0, 0);
    } else {
        runNeedlemanWunsch(type_v, type_w, string_v, string_w, none, threads, 0, 0);
    }

}

void on_accept_clicked(GtkButton *button, gpointer data){
    char* selected = gtk_combo_box_text_get_active_text(comboBox);
    printf("selected: %s", selected);
    threads = atoi(gtk_entry_get_text (cantThreads));
    string_v = gtk_entry_get_text (h1);
    string_w = gtk_entry_get_text (h2);
    int error = 0;
    printf("%d\n", threads);
    if(threads < 1 || threads > 100){
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(entrada),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Threads must be > 1 and < 100");
        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        error = 1;
    }

    int length = gtk_entry_get_text_length(gap);
    if(length == 0){
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(entrada),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Must set gap penalty");
        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        error = 1;
    }
    score_table.gap = atoi(gtk_entry_get_text(gap));

    if(score_table.type == 0){
      if(gtk_entry_get_text_length(match) > 0){
          score_table.match = atoi(gtk_entry_get_text(match));
      } else {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(entrada),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Must set match or load a table");
        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        error = 1;
      }
      if(gtk_entry_get_text_length(miss) > 0){
          score_table.missmatch = atoi(gtk_entry_get_text(miss));
      } else {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(entrada),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Must set missmatch or load a table");
        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        error = 1;
      }
    }

    if(error == 0){
        if(selected[0] == 'N'){
            gtk_widget_show(nw);
        } else if(selected[0] == 'S') {
            gtk_widget_show(sw);
        }
    }
}


void on_loadH1_clicked(GtkButton *button, gpointer data){
    GtkWidget *dialog;
    char *filename;
    dialog = gtk_file_chooser_dialog_new ("Cargar Archivo",
                                          NULL,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "_Cancel", GTK_RESPONSE_CANCEL,
                                          "_Open", GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
        gboolean result;
        GError *err = NULL;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        string_v = readStringFromFile(filename);
        gtk_entry_set_text (h1, string_v);
        free(filename);
    }
    gtk_widget_destroy (dialog);

}

void on_loadH2_clicked(GtkButton *button, gpointer data){
    GtkWidget *dialog;
    char *filename;
    dialog = gtk_file_chooser_dialog_new ("Cargar Archivo",
                                          NULL,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "_Cancel", GTK_RESPONSE_CANCEL,
                                          "_Open", GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
        gboolean result;
        GError *err = NULL;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        string_w = readStringFromFile(filename);
        gtk_entry_set_text (h2, string_w);
        free(filename);
    }
    gtk_widget_destroy (dialog);
}

void on_loadTable_clicked(GtkButton *button, gpointer data){
    GtkWidget *dialog;
    char *filename;
    dialog = gtk_file_chooser_dialog_new ("Cargar Archivo",
                                          NULL,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "_Cancel", GTK_RESPONSE_CANCEL,
                                          "_Open", GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
        gboolean result;
        GError *err = NULL;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        readMatrixFromFile(filename);
        g_free(filename);
    }
    gtk_widget_destroy (dialog);
}


void hide_on_close(GtkWidget *widget, GdkEventAny *event){
    gtk_widget_hide (widget);
}


int main(int argc, const char *argv[]) {

    GError *error = NULL;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    if (!gtk_builder_add_from_file(builder, "entrada.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
        return (1);
    }
    if (!gtk_builder_add_from_file(builder, "nw.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
        return (1);
    }
    if (!gtk_builder_add_from_file(builder, "sw.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
        return (1);
    }

    getGtkWidgets(builder);
    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(G_OBJECT(entrada), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(sw), "delete_event",
                    G_CALLBACK(hide_on_close), NULL);

    g_signal_connect(G_OBJECT(nw), "delete_event",
            G_CALLBACK(hide_on_close), NULL);
	setlocale (LC_ALL,"C");

    g_object_unref(G_OBJECT(builder));
    gtk_widget_show(entrada);
    gtk_main();

    return 0;
}
