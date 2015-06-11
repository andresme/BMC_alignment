#include "globals.h"
#include "utils.h"
#include "alignments.h"
#include <gtk/gtk.h> 

GtkWidget *entrada, *nw, *sw;
GtkEntry  *h1, *h2, *cantThreads ,*match, *miss, *gap, *valueK, *ampB, *valueg, *valueh;
GtkWidget *matriz, *results;
GtkTextBuffer *buffer, *bufferResults;
GtkBuilder *builder;



int main(int argc, const char* argv[]) {
	
	GError *error = NULL;
	gtk_init(&argc, &argv);
	
    builder = gtk_builder_new();
    
    if(!gtk_builder_add_from_file(builder, "entrada.glade", &error)) {
		g_warning("%s", error->message);
		g_free(error);
		return(1);
	}
   	getGtkWidgets(builder);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(entrada);
	gtk_main();
		
	return 0;

    score_table.gap = -2;
    score_table.match = 1;
    score_table.missmatch = -1;
    score_table.new_block_cost = -2;
    score_table.continue_block_cost = -1;

//    for(int i = 1; i <= 4; i++){
        runNeedlemanWunsch(penalty_left_penalty_right, penalty_left_penalty_right, "TCTAACGGTAAAATCATCATCAAA", "TCTAACGGTAAAATCATCATCAAA", k_band, 4, 0, 1);
        //runSmithWaterman("TCTAACGGTAAAATCATCATCAAA", "CGCATCTCGA", none, 4);
//    }


    pthread_exit(NULL);
}


void getGtkWidgets(GtkBuilder *builder){
		entrada = GTK_WIDGET(gtk_builder_get_object(builder, "entrada"));
		cantThreads = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "threads"));
		h1 = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "h1"));
		h2 = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "h2"));
		match = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "match"));
		miss = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "miss"));
		gap = (GtkEntry *) GTK_WIDGET(gtk_builder_get_object(builder, "gap"));

		
	}
