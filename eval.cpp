#include <unordered_map>
#include <algorithm>
#include <string>
#include <list>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"

eval_t::eval_t(syntax_tree_t &copied_tree) {
	form_tree_         = new syntax_tree_t();
	form_tree_->state_ = copied_tree.state_ ;
	form_tree_->root_  = copied_tree.copy_tree_root(copied_tree.root_);

	form_tree_->root_ = parse_eval (form_tree_->root_) ;
	form_tree_->root_ = second(form_tree_->root_);
	form_tree_->show();
}

syntax_tree_t eval_t::evaluation(std::string data){
	lex_array_t lexems (data.data());
	std::unordered_map < std::string, bool > variables_table_;

	int state = 0;
	while (state < lexems.size_ - 2) {
		switch (lexems.lexems_[state].kind) {
			case VAR:
				++state;
				if (lexems.lexems_[state].kind == OP && lexems.lexems_[state].lex.op == EQUAL)
					++state;
				else {
					std::cout << "eval - unexpected lexem" << 
							 ": expected EQUAL operation"<< std::endl;
					abort();
				}
				if (lexems.lexems_[state].kind == T || lexems.lexems_[state].kind == F)
					variables_table_[std::string(lexems.lexems_[state - 2].lex.var)] = lexems.lexems_[state].lex.value;
				else {
					std::cout << "eval - unexpected lexem" << 
							 ": expected true or false value"<< std::endl;
					abort();
				}
				++state;
				break;
			default:
				std::cout << lexems.lexems_[state] << "\t" ;
				std::cout << "eval lexer error - unexpected lexem" <<
							 ": expected VariableName"<< std::endl;
				abort();
		}
	}

	node_t *eval_root_ = form_tree_->copy_tree_root(form_tree_->root_);
	eval_root_ = translate (eval_root_, variables_table_);
	eval_root_ = parse_eval(eval_root_);

	syntax_tree_t answ(eval_root_);
	//answ.show();

	return answ;
} 

node_t *eval_t::parse_eval (node_t *current) {
	if (current         == nullptr) return nullptr;
	if (current->data.k != NODE_OP) return current;

	if (current->data.u.op == IMPL) {
		current->data.u.op  = OR;
		node_t *new_node = new node_t ();
		new_node->data.k    = NODE_OP;
		new_node->right     = nullptr;
		new_node->left      = current->left;
		new_node->data.u.op = NOT;
		new_node->isbracket = true;
		
		current->left = new_node;
		return parse_eval(current);
	}
	
	if (current->data.u.op == NOT) {
		if (current->left->data.k == NODE_OP && current->left->data.u.op == NOT) {
			current = current->left->left;
			return parse_eval(current);
		}
		node_t *temp = parse_eval(current->left);

		if (temp->data.k != NODE_VAL) {
			current->left = temp;
			return current;
		}
		node_t *new_node = new node_t ();
		new_node->left   = nullptr;
		new_node->right  = nullptr;
		new_node->data.k = NODE_VAL;
		new_node->data.u.val = 1 - temp->data.u.val;
		new_node->data.value = new_node->data.u.val ;
		new_node->isbracket  = true;
		
		return new_node;
	}

	node_t *temp1 = parse_eval(current->left), *temp2 =  parse_eval(current->right) ;

	if (temp1->data.k != NODE_VAL || temp2->data.k != NODE_VAL) {

		if (current->data.u.op == AND && (temp1->data.value == 0 || temp2->data.value == 0)) {
			node_t *new_node = new node_t ();
			new_node->left   = nullptr;
			new_node->right  = nullptr;
			new_node->data.k = NODE_VAL;
			new_node->data.u.val = 0;
			new_node->data.value = 0;
			new_node->isbracket  = true;

			return new_node;
		}

		if (current->data.u.op == AND && (temp1->data.value == 1 || temp2->data.value == 1)) {
			if (temp1->data.value == 1) return temp2;
				else return temp1;
		}

		if ((current->data.u.op == OR   && (temp1->data.value == 1 || temp2->data.value == 1)) ||
			(current->data.u.op == IMPL && (temp1->data.value == 0 || temp2->data.value == 1))) {
				node_t *new_node = new node_t ();
				new_node->left   = nullptr;
				new_node->right  = nullptr;
				new_node->data.k = NODE_VAL;
				new_node->data.u.val = 1;
				new_node->data.value = 1;
				new_node->isbracket  = true;

				return new_node;
		}

		if (current->data.u.op == OR && (temp1->data.value == 0 || temp2->data.value == 0)) {
			if (temp1->data.value == 0)
				return temp2;
			else
				return temp1;
		}
		
		if (current->data.u.op == IMPL && temp1->data.value == 1) {
			return temp2;
		}

		if (current->data.u.op == IMPL && temp2->data.value == 0) {
			node_t *new_node    = new node_t();
			new_node->left      = temp1;
			new_node->right     = nullptr;
			new_node->data.k    = NODE_OP;
			new_node->data.u.op = NOT;
			temp1->isbracket    = true;

			return new_node;
		}

		current->left  = temp1;
		current->right = temp2;
		current->isbracket  = true;
		return current;
	}

	node_t *new_node = new node_t ();
	new_node->left   = nullptr;
	new_node->right  = nullptr;
	new_node->data.k = NODE_VAL;

	switch (current->data.u.op) {
		case AND:	
			new_node->data.u.val = ((temp1->data.u.val) & (temp2->data.u.val));
			break;

		case OR:
			new_node->data.u.val = (temp1->data.u.val) | (temp2->data.u.val);
			break;

		case IMPL:
			if (temp1->data.u.val && !temp2->data.u.val) new_node->data.u.val = 0;
				else new_node->data.u.val = 1;
			break;	
		default:
			std::cout << "unknown operation" << std::endl;
			abort();
	}
	new_node->data.value = new_node->data.u.val ; 
	new_node->isbracket  = true;
	current = new_node;
	return new_node;
}

node_t *eval_t::second(node_t *current) {
	if (current == nullptr) return nullptr;
	current->left  = second(current->left );
	if (current->data.k == NODE_OP && current->data.u.op == NOT)  {
		return de_morgans_laws (current->left);
	}
	current->right = second(current->right);
	return current;
}

node_t *eval_t::de_morgans_laws (node_t *current) {
	if (current == nullptr) return nullptr;

	if (current->data.k == NODE_OP) { 
		if (current->data.u.op == AND) {
			current->data.u.op = OR;
		}
		else if (current->data.u.op == OR) {
			current->data.u.op = AND;
		}
		else if (current->data.u.op == NOT) {
			return current->left;
		}
		current->left  = de_morgans_laws(current->left );
		current->right = de_morgans_laws(current->right);
		return current;
	}
	
	node_t *new_node = new node_t ();
	new_node->data.k    = NODE_OP;
	new_node->data.u.op = NOT;
	new_node->right     = nullptr;
	new_node->left      = current;
	new_node->isbracket = true;

	current = new_node;

	return current;
}

node_t *eval_t::translate(node_t *current, std::unordered_map < std::string, bool > variables_table) {
	if (current == nullptr) return nullptr;

	if (current->data.k == NODE_VAR && 
	    variables_table.find(std::string(current->data.u.var)) != variables_table.end()) {
			current->data.k     = NODE_VAL ;
			current->data.u.val = variables_table[std::string(current->data.u.var)] ;
			current->data.value = current->data.u.val ;
	}

	current->left  = translate(current->left , variables_table);
	current->right = translate(current->right, variables_table);

	return current;
}

std::list < std::string > *eval_t::create_variables_list(node_t *root) {
	std::list < std::string > *new_list = new std::list < std::string > ();
	inorder_create_list(root , new_list);
	return new_list;
}

void inorder_create_list(node_t *current, std::list < std::string > * lst) {
	if (current == nullptr) return;
	if (current->data.k == NODE_VAR && std::find(lst->begin(), lst->end(), std::string(current->data.u.var)) == lst->end()) {
			lst->push_back(current->data.u.var);
	}
	inorder_create_list(current->left , lst);
	inorder_create_list(current->right, lst);
}

eval_t::~eval_t() {
	form_tree_->destroy_syntax_tree_t(form_tree_->root_);
}

bool eval_t::is_TAUT() {
	std::list < std::string > * variable_list = create_variables_list(form_tree_->root_);
	int var_count = variable_list->size();
	unsigned long long int r_flags = 0ll;
	std::string temp = "" ;

	while (((r_flags >> var_count) & 1) != 1) {
		int cur = 0;
		temp = "";
		for (auto it: *variable_list) {
			temp += it + '=' + (char)(((r_flags >> cur) & 1) + '0') + ' ';
			++cur;
		}
		//std::cout << "!" <<  temp << "!" << std::endl;
		syntax_tree_t answ = evaluation(temp);
		if (!answ.root_->data.value) {
			std::cout << temp << "=> ";
			return false;
		}
		++r_flags;
	}

	return true;
}

bool eval_t::is_SAT() {
	std::list < std::string > * variable_list = create_variables_list(form_tree_->root_);
	int var_count = variable_list->size();
	unsigned long long int r_flags = 0ll;
	std::string temp = "" ;

	while (((r_flags >> var_count) & 1) != 1) {
		int cur = 0;
		temp = "";
		for (auto it: *variable_list) {
			temp += it + '=' + (char)(((r_flags >> cur) & 1) + '0') + ' ';
			++cur;
		}
		//std::cout << "!" <<  temp << "!" << std::endl;
		syntax_tree_t answ = evaluation(temp);
		if (answ.root_->data.value) {
			std::cout << temp << "=> " ;
			return true;
		}
		++r_flags;
	}

	return false;
}