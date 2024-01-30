#include "multi_behavior.h"

void MultiBehavior::push_left(MultiBehavior *instance) {
	if (instance == this || instance == nullptr) return;

	instance->next = this;
	instance->previous = previous;
	if (previous) previous->next = instance;
	previous = instance;
}

void MultiBehavior::push_right(MultiBehavior *instance) {
	if (instance == this || instance == nullptr) return;

	instance->previous = this;
	instance->next = next;
	if (next) next->previous = instance;
	next = instance;
}

void MultiBehavior::push_up(MultiBehavior *instance) {
	if (instance == this || instance == nullptr) return;

	instance->low = this;
	instance->high = high;
	if (high) high->low = instance;
	high = instance;
}

void MultiBehavior::push_down(MultiBehavior *instance) {
	if (instance == this || instance == nullptr) return;

	instance->high = this;
	instance->low = low;
	if (low) low->high = instance;
	low = instance;
}


/// <summary>
/// ポップ
/// </summary>
/// <param name="instance"></param>
void MultiBehavior::pop(MultiBehavior *instance) {
	if (instance->parent != this) return;

	// ヘッダ、テールの更新
	if (head == instance && head->next)
		head = head->next;
	else if (tail == instance && tail->previous)
		tail = tail->previous;


	// プライオリティのリンク更新
	if (instance->next && instance->next->priority == instance->priority) {
		if (instance->high) instance->high->low = instance->next;
		if (instance->low) instance->low->high = instance->next;

		if (instance->next) instance->next->high = instance->high;
		if (instance->next) instance->next->low = instance->low;
	}
	else {
		if (instance->high) instance->high->low = instance->low;
		if (instance->low) instance->low->high = instance->high;
	}

	// リンクを外す
	if (instance->next) instance->next->previous = instance->previous;
	if (instance->previous) instance->previous->next = instance->next;
	instance->next = instance->previous = instance->high = instance->low = nullptr;
	instance->parent = nullptr;



	// カウントを減らす
	count--;
	if (count <= 0) {
		head = tail = nullptr;
		count = 0;
	}
}

/// <summary>
/// プッシュ
/// </summary>
void MultiBehavior::push(MultiBehavior *instance) {
	if (instance->parent != nullptr) return;
	instance->parent = this;

	// 初期
	if (count == 0) {
		head = tail = instance;
	}
	// 二回目
	else {

		MultiBehavior *current = head;
		// 登録する場所を探索する
		while (current->priority < instance->priority) {
			if (current->low) current = current->low;
			else break;
		}





		// 優先ポインタが登録されているなら、右に追加する
		if (instance->priority == current->priority) {
			bool isTail = current->next == nullptr;

			// 登録
			current->push_right(instance);

			// 末尾なら、ポインタを更新
			if (isTail) tail = instance;
		}

		// 優先ポインタが登録されていないなら、左に追加する
		else if (instance->priority < current->priority) {
			bool isHead = current->high == nullptr;

			// 登録
			current->push_up(instance);
			current->push_left(instance);

			// 先頭なら、ポインタを更新
			if (isHead) {
				head = instance;
			}
		}

		// 登録する場所がないなら、末尾に登録する
		else {
			current->push_down(instance);
			tail->push_right(instance);

			// 末尾なら、ポインタを更新
			tail = instance;
		}
	}


	count++;
}


MultiBehavior::~MultiBehavior() {
	MultiBehavior *current = head;
	while (current) {
		MultiBehavior *next = current->next;
		// 削除
		delete current;
		current = next;
	}
	head = nullptr;
	count = 0;
}

std::list<MultiBehavior *> MultiBehavior::Find(MultiBehavior *(*func)(MultiBehavior *instance)) {
	std::list<MultiBehavior *> result;
	for (auto &instance : *this) {
		MultiBehavior *rst = func(&instance);
		if (rst) result.push_back(rst);
	}
	return result;
};

void MultiBehavior::Destroy(void) {
	if (!isDestroy && parent) {
		parent->deletes.push_back(this);
		isDestroy = true;
	}
}

void MultiBehavior::AllLoop(void) {
	// 登録
	while (creates.size()) {
		// 登録
		push(creates.front());
		creates.front()->Initialize();
		creates.pop_front();
	}



	// 更新
	Loop();
	for (MultiBehavior *current = head; current; current = current->next) {
		current->AllLoop();
	}



	// 削除
	while (deletes.size()) {
		// 削除
		pop(deletes.front());
		deletes.front()->Release();
		delete deletes.front();
		deletes.pop_front();
	}
}

void MultiBehavior::SetParent(MultiBehavior *parent) {
	if (parent && parent == this->parent) return;

	if (this->parent && !isDestroy) {
		// 親外し
		this->parent->pop(this);
	}

	// 親の切り替え
	if (parent) {
		parent->push(this);
	}
}
