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
/// �|�b�v
/// </summary>
/// <param name="instance"></param>
void MultiBehavior::pop(MultiBehavior *instance) {
	if (instance->parent != this) return;

	// �w�b�_�A�e�[���̍X�V
	if (head == instance && head->next)
		head = head->next;
	else if (tail == instance && tail->previous)
		tail = tail->previous;


	// �v���C�I���e�B�̃����N�X�V
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

	// �����N���O��
	if (instance->next) instance->next->previous = instance->previous;
	if (instance->previous) instance->previous->next = instance->next;
	instance->next = instance->previous = instance->high = instance->low = nullptr;
	instance->parent = nullptr;



	// �J�E���g�����炷
	count--;
	if (count <= 0) {
		head = tail = nullptr;
		count = 0;
	}
}

/// <summary>
/// �v�b�V��
/// </summary>
void MultiBehavior::push(MultiBehavior *instance) {
	if (instance->parent != nullptr) return;
	instance->parent = this;

	// ����
	if (count == 0) {
		head = tail = instance;
	}
	// ����
	else {

		MultiBehavior *current = head;
		// �o�^����ꏊ��T������
		while (current->priority < instance->priority) {
			if (current->low) current = current->low;
			else break;
		}





		// �D��|�C���^���o�^����Ă���Ȃ�A�E�ɒǉ�����
		if (instance->priority == current->priority) {
			bool isTail = current->next == nullptr;

			// �o�^
			current->push_right(instance);

			// �����Ȃ�A�|�C���^���X�V
			if (isTail) tail = instance;
		}

		// �D��|�C���^���o�^����Ă��Ȃ��Ȃ�A���ɒǉ�����
		else if (instance->priority < current->priority) {
			bool isHead = current->high == nullptr;

			// �o�^
			current->push_up(instance);
			current->push_left(instance);

			// �擪�Ȃ�A�|�C���^���X�V
			if (isHead) {
				head = instance;
			}
		}

		// �o�^����ꏊ���Ȃ��Ȃ�A�����ɓo�^����
		else {
			current->push_down(instance);
			tail->push_right(instance);

			// �����Ȃ�A�|�C���^���X�V
			tail = instance;
		}
	}


	count++;
}


MultiBehavior::~MultiBehavior() {
	MultiBehavior *current = head;
	while (current) {
		MultiBehavior *next = current->next;
		// �폜
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
	// �o�^
	while (creates.size()) {
		// �o�^
		push(creates.front());
		creates.front()->Initialize();
		creates.pop_front();
	}



	// �X�V
	Loop();
	for (MultiBehavior *current = head; current; current = current->next) {
		current->AllLoop();
	}



	// �폜
	while (deletes.size()) {
		// �폜
		pop(deletes.front());
		deletes.front()->Release();
		delete deletes.front();
		deletes.pop_front();
	}
}

void MultiBehavior::SetParent(MultiBehavior *parent) {
	if (parent && parent == this->parent) return;

	if (this->parent && !isDestroy) {
		// �e�O��
		this->parent->pop(this);
	}

	// �e�̐؂�ւ�
	if (parent) {
		parent->push(this);
	}
}
