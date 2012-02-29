#include "automatawindow.h"

#include <cstdlib>
#include <iostream>

AutomataWindow::AutomataWindow(short init_size, short init_rule):
	growth_area(init_size, init_rule),
	five_button("next five states"),
	twenty_button("next twenty states"),
	screen_button("next screen"),
	control_frame("rule control"),
	msg_label("enter the rule [0..255]:"),
	set_button("Apply")
{
	this->set_default_size(init_size, init_size);	
	this->set_border_width(15);
	this->set_title("Wolfram's cellular automata model");
	this->set_resizable(false);	

	// it gets the picture in current file 
	this->set_icon_from_file(Glib::ustring("./black_white_shell.gif"));

	// add the main box to main window
	this->add(main_box);

	// main box children
	main_box.add(growth_area);	
	main_box.add(widgets_box);

	// widgets box children
	widgets_box.add(button_box);
	widgets_box.add(control_frame);

	// button box children
	five_button.signal_clicked().connect(sigc::mem_fun(*this, &AutomataWindow::on_five_button_clicked));
	button_box.pack_start(five_button, Gtk::PACK_SHRINK);

	twenty_button.signal_clicked().connect(sigc::mem_fun(*this, &AutomataWindow::on_twenty_button_clicked));
	button_box.pack_start(twenty_button, Gtk::PACK_SHRINK);

	screen_button.signal_clicked().connect(sigc::mem_fun(*this, &AutomataWindow::on_screen_button_clicked));
	button_box.pack_start(screen_button, Gtk::PACK_SHRINK);

	button_box.set_border_width(15);

	// control frame children
	control_frame.add(rule_box);

	// rule box children
	rule_box.pack_start(msg_label, Gtk::PACK_EXPAND_WIDGET, 5);
	rule_box.pack_start(entry_rule, Gtk::PACK_EXPAND_WIDGET, 5);

	entry_rule.set_increments(1.0, 1.0);
	entry_rule.set_range(0.0, 255.0);
	entry_rule.set_value(init_rule);
	entry_rule.grab_focus();

	set_button.set_border_width(5);
	set_button.signal_clicked().connect(sigc::mem_fun(*this, &AutomataWindow::on_set_button_clicked));
	rule_box.pack_start(set_button, Gtk::PACK_EXPAND_WIDGET, 15);

	this->show_all_children();
}

void AutomataWindow::on_five_button_clicked()
{
	int step;

	for (int count = 0; count < 5; count++)
	{
		step = growth_area.get_end();

		if (step == growth_area.get_bounds() - 1)
			step = 0;
		else
			step++;

		growth_area.apply_rule(step);
	}

	// redraws the drawing area
	this->queue_draw();
}

void AutomataWindow::on_twenty_button_clicked()
{
	int step;

	for (int count = 0; count < 20; count++)
	{
		step = growth_area.get_end();

		if(step == growth_area.get_bounds() - 1)
			step = 0;
		else
			step++;

		growth_area.apply_rule(step);
	}

	// redraws the drawing area
	this->queue_draw();
}

void AutomataWindow::on_screen_button_clicked()
{
	int step;

	for (int count = 0; count < growth_area.get_bounds(); count++)
	{
		step = growth_area.get_end();

		if (step == growth_area.get_bounds() - 1)
			step = 0;
		else
			step++;

		growth_area.apply_rule(step);
	}

	// redraws the drawing area
	this->queue_draw();
}

void AutomataWindow::on_set_button_clicked()
{
	short new_rule;

	new_rule = entry_rule.get_value_as_int();

	growth_area.set_rule(new_rule);

	// applies the rule for the entire screen
	for (int step = 1; step < growth_area.get_bounds(); step++)
		growth_area.apply_rule(step);

	entry_rule.grab_focus();

	// redraws the drawing area only and only if new_rule is a valid rule
	this->queue_draw();
}
